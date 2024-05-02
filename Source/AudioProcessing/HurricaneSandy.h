#pragma once

#include "../shameConfig.h"
#include "EnvelopeDips.h"
#include "Granulate.h"
#include "LoopCrossfade.h"
#include "Biquads.h"
#include "Envelope.h"

class HurricaneSandy
{
public:
    HurricaneSandy() : grainImpact(0.0f), ampFluctuationImpact(0.0f)
    {
        rng.setSeedRandomly();

        // Used for general amplitude fluctuation
        dips.setDomainMS(1000.0f);  // milliseconds
        dips.setDynamicExtremity(0.5f);
        dips.setNumPoints(15);
        dips.setNumPointRandomness(0.5f);
        dips.calculateDipPoints();

        // Used to mix noise burst
        noiseEnv.addEnvelopePoint(0.143f, 0.073f);
        noiseEnv.addEnvelopePoint(0.305f, 0.367f);
        noiseEnv.addEnvelopePoint(0.383f, 0.567f);
        noiseEnv.addEnvelopePoint(0.428f, 0.0f);

        // For ducking the original signal during noise burst
        sigEnv.addEnvelopePoint(0.143f, 0.2f);
        sigEnv.addEnvelopePoint(0.305f, 0.8f);
        sigEnv.addEnvelopePoint(0.383f, 0.5f);
        sigEnv.addEnvelopePoint(0.428f, 0.0f);

        // Used to modulate the amplitude of the incoming signal
        granulator.setRandomFactor(1.0f);
        granulator.setStretch(0);
        granulator.setGrainParameters(5, 50, 50, 50);
        granulator.setVoices(10);

        // Pre-recorded low frequency granular noise
        lowFreqGranular.setLoopCrossfadeLevel(0.25);

        // Filters for granulation and the main signal
        lpButterworth_Grains.setButterworth_LowHighPass(2000.0f, true);
        hpButterworth_Grains.setButterworth_LowHighPass(50.0f, false);
        lpButterworth_Signal.setButterworth_LowHighPass(22050.0f, true);

        // Initialize parameters
        setInterpolatedParameters(0.0f);
    }

    void prepareToPlay(float sampleRate) noexcept
    {
        dips.prepareToPlay(sampleRate);
        //granulator.prepareToPlay(sampleRate);  // TODO
        lowFreqGranular.prepareToPlay(sampleRate);
        lpButterworth_Grains.prepareToPlay(sampleRate);
        hpButterworth_Grains.prepareToPlay(sampleRate);
        lpButterworth_Signal.prepareToPlay(sampleRate);
        noiseEnv.prepareToPlay(sampleRate);
        sigEnv.prepareToPlay(sampleRate);
    }

    void reset() noexcept
    {
        dips.reset();
        //granulator.reset();  // TODO: test this!
        lowFreqGranular.reset();
        lpButterworth_Grains.reset();
        hpButterworth_Grains.reset();
        lpButterworth_Signal.reset();
        noiseEnv.reset();
        sigEnv.reset();
    }

    void processHurricaneSandy(juce::AudioBuffer<float>& buffer, int numChannels)
    {
        for (int i = 0; i < buffer.getNumSamples(); ++i) {

            float lfGrainSample = lowFreqGranular.processLoopCrossSample(0);

            float grainSample = granulator.tick();
            grainSample = lpButterworth_Grains.process(grainSample, 0);
            grainSample = hpButterworth_Grains.process(grainSample, 0);

            float dipsLevel = dips.processEnvelopeDips();

            float noiseBurstEnvValue = noiseEnv.processEnvelope();
            float signalEnvValue = 1.0f - sigEnv.processEnvelope();

            for (int channel = 0; channel < numChannels; ++channel) {
                float* samples = buffer.getWritePointer(channel);

                // This was commented out in the original code.
                //float bufferMagnitude = buffer.getMagnitude(channel, 0, buffer.getNumSamples());
                //float scaledGrainSample = grainSample*bufferMagnitude;

                // Low-pass filter the original signal.
                if (channel < 2) {
                    samples[i] = lpButterworth_Signal.process(samples[i], channel);
                }

                // Mix signal with periodic noise burst.
                float noiseBurst = signalEnvValue * samples[i] + 0.05f * noiseBurstEnvValue * (rng.nextFloat() * 2.0f - 1.0f);
                samples[i] = (1.0f - noiseBurstImpact)*samples[i] + noiseBurstImpact*noiseBurst;

                // Modulate the amplitude by the granular amplitude
                samples[i] *= 1.0f - grainImpact*grainSample;

                // Fluctuate the overall amplitude
                samples[i] = ampFluctuationImpact*dipsLevel*samples[i] + (1.0f - ampFluctuationImpact)*samples[i];

                // Mix signal with all of the low freq grain noise
                samples[i] = samples[i] + lowFreqGrainNoiseLevel*lfGrainSample;
            }
        }

        // Needs: Maybe just a little reverb to blend the sounds...
    }

    void setInterpolatedParameters(float input)
    {
        if (input < 0.0f) { input = 0.0f; }
        if (input > 1.0f) { input = 1.0f; }

        // Grain interpolation
        grainImpact = 1.0f * input;
        granulator.setGrainParameters((unsigned int)(10.0f * input + 5.0f), 75, 50,
                                      (unsigned int)(1000.0f * (1.01f - input)));

        // Amount of low frequency granular noise mixed in
        lowFreqGrainNoiseLevel = 0.15f * input;

        // Amplitude fluctuation
        ampFluctuationImpact = input;

        // Periodic bursts of white noise
        if (input > 0.5f) {
            noiseBurstImpact = 2.0f*(input - 0.5f);
        } else {
            noiseBurstImpact = 0.0f;
        }

        // Low-pass filtering of original signal. Sets the cutoff frequency
        // linearly between 22050 Hz and 2000 Hz.
        lpButterworth_Signal.setButterworth_LowHighPass(20050.0f*(1.0f - input) + 2000.0f, true);
    }

private:
    EnvelopeDips dips;
    Granulate granulator { 5, BinaryData::PinkNoise_wav, BinaryData::PinkNoise_wavSize };
    LoopCrossfade lowFreqGranular;
    Biquads lpButterworth_Grains;
    Biquads hpButterworth_Grains;
    Biquads lpButterworth_Signal;

    Envelope noiseEnv { 350 };
    Envelope sigEnv { 350 };

    float grainImpact;
    float lowFreqGrainNoiseLevel;
    float ampFluctuationImpact;
    float noiseBurstImpact;

    juce::Random rng;
};
