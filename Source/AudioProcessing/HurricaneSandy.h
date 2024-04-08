#pragma once

#include "../shameConfig.h"
//#include "EnvelopeDips.h"
//#include "Granulate.h"
//#include "Loop.h"
#include "LoopCrossfade.h"
#include "Biquads.h"
//#include "Envelope.h"
//#include "Noise.h"

class HurricaneSandy
{
public:
    HurricaneSandy() : grainImpact(0.0f), ampFluctuationImpact(0.0f)
    {
        // Used for general amplitude fluctuation
//        dips.reset(new EnvelopeDips());
//        dips->setDomainMS(1000); //NOTE: domain is set in milleseconds
//        dips->setDynamicExtremity(0.5);
//        dips->setNumPoints(15);
//        dips->setNumPointRandomness(0.5);
//        dips->calculateDipPoints();

        // Used to mix noise burst
//        whiteNoise.reset(new Noise());
//
//        noiseEnv.reset(new Envelope(350));
//        noiseEnv->addEvelopePoint(0.143, 0.073);
//        noiseEnv->addEvelopePoint(0.305, 0.367);
//        noiseEnv->addEvelopePoint(0.383, 0.567);
//        noiseEnv->addEvelopePoint(0.428, 0.0);
//
//        sigEnv.reset(new Envelope(350));
//        sigEnv->addEvelopePoint(0.143, 0.2);
//        sigEnv->addEvelopePoint(0.305, 0.8);
//        sigEnv->addEvelopePoint(0.383, 0.5);
//        sigEnv->addEvelopePoint(0.428, 0.0);


        // Used to modulate the amplitude of the incomming signal
//        int numVoices = 5;
//        granulator.reset(new Granulate(numVoices, AUDIO_PATH + "PinkNoise.wav"));
//        granulator->setRandomFactor(1.0);
//        granulator->setStretch(0);
//        granulator->setGrainParameters(5, 50, 50, 50);
//        granulator->setVoices(10);

        // Pre-recorded low frequency granular noise
        lowFreqGranular.setLoopCrossfadeLevel(0.25);

        // Filters for granulation and the main signal
        lpButterworth_Grains.setButterworth_LowHighPass(2000.0f, true);
        hpButterworth_Grains.setButterworth_LowHighPass(50.0f, false);
        lpButterworth_Signal.setButterworth_LowHighPass(22050.0f, true);

        // Initialize parameters
        setInterpolatedParameters(0.0f);
    }

    //Needs:
    //Filtering!!!!
    //- LP on dry signal.
    //- Buttorsworth LP + HP on granulated noise
    //Ducking:
    //- dry signal ducks in sync with noise burst
    //Reverb:
    //Maybe just a little reverb to blend the sounds...


    void processHurricaneSandy(juce::AudioBuffer<float>& buffer, int numChannels)
    {
        for (int i = 0; i < buffer.getNumSamples(); ++i) {

            float lfGrainSample = lowFreqGranular.processLoopCrossSample(0);
//            float grainSample = granulator->tick();
//
//            grainSample = lpButterworth_Grains->process(grainSample, 0);
//            grainSample = hpButterworth_Grains->process(grainSample, 0);
//
//
//            float dipsLevel = dips->processEnvelopeDips();
//
//            float noiseBurstEnvValue = noiseEnv->processEnvelope();
//            float signalEnvValue = (1 - sigEnv->processEnvelope());


            for (int channel = 0; channel < numChannels; ++channel) {
                float* samples = buffer.getWritePointer(channel);

                // This was commented out in the original code.
                //float bufferMagnitude = buffer.getMagnitude(channel, 0, buffer.getNumSamples());
                //float scaledGrainSample = grainSample*bufferMagnitude;

/*
                // Low-pass filter the original signal.
                if (channel < 2) {
                    samples[i] = lpButterworth_Signal.process(samples[i], channel);
                }
*/

//                //mix signal with periodic noise burst.
//                samples[i] = (1 - noiseBurstImpact)*samples[i] + noiseBurstImpact*(signalEnvValue*samples[i] + 0.05*noiseBurstEnvValue*whiteNoise->tick());
//
//                //Modulate the amplitude by the granular amplitude
//                samples[i] *= (1 - grainImpact*grainSample);
//
//
//                //Fluctuate the overall amplitude
//                samples[i] = ampFluctuationImpact*dipsLevel*samples[i] + (1 - ampFluctuationImpact)*samples[i];


                // Mix signal with all of the low freq grain noise
                samples[i] = samples[i] + lowFreqGrainNoiseLevel*lfGrainSample;
            }
        }
    }

    void setInterpolatedParameters(float input)
    {
        if (input < 0.0f) { input = 0.0f; }
        if (input > 1.0f) { input = 1.0f; }

//        //grain interpolation
//        grainImpact = 1.0 * input;
//        granulator->setGrainParameters(10*input + 5, 75, 50, 1000*(1.01 - input));

        // Amount of low frequency granular noise mixed in
        lowFreqGrainNoiseLevel = 0.15f * input;

//        //Amplitude fluctuation
//        ampFluctuationImpact = input;
//
//        //Amplitude fluctuation
//        if(input > 0.5)
//            noiseBurstImpact = 2*(input - 0.5);

        // Low-pass filtering of original signal. Sets the cutoff frequency
        // linearly between 22050 Hz and 2000 Hz.
        lpButterworth_Signal.setButterworth_LowHighPass(20050.0f*(1.0f - input) + 2000.0f, true);
    }

private:
//    std::unique_ptr<EnvelopeDips> dips;
//    std::unique_ptr<Granulate> granulator;
    LoopCrossfade lowFreqGranular;
    Biquads lpButterworth_Grains;
    Biquads hpButterworth_Grains;
    Biquads lpButterworth_Signal;

//    std::unique_ptr<Noise> whiteNoise;
//    std::unique_ptr<Envelope> noiseEnv;
//    std::unique_ptr<Envelope> sigEnv;

    float grainImpact;
    float lowFreqGrainNoiseLevel;
    float ampFluctuationImpact;
    float noiseBurstImpact;
};
