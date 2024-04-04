#pragma once

#include "../shameConfig.h"

class InputSaturation
{
public:
    InputSaturation(float threshold, float rateOdd, float rateEven)
    {
        setThreshold(threshold);
        setRateOdd(rateOdd);
        setRateEven(rateEven);
        setOutput(1.0f);

        drive = 1.0f;
        oddGain = 1.0f;
        evenGain = 0.3f;

        // TODO: put this into a reset() function
        for (int i = 0; i < 10; ++i) {
            priorSample[i] = 0.0f;
        }

        // TODO: put this into a prepareToPlay() function
        coef = 0.0f;
        setFrequencyRolloff(4000.0f);
    }

    void setThreshold(float threshold)
    {
        if (threshold > 1.0f) {
            satThreshold = 1.0f;
        } else if (threshold < 0.0f) {
            satThreshold = 0.0f;
        } else {
            satThreshold = threshold;
        }
    }

    void setRateOdd(float rate)
    {
        satRateOdd = (rate < 0.0f) ? 0.0f : rate;
    }

    void setRateEven(float rate)
    {
        satRateEven = (rate < 0.0f) ? 0.0f : rate;
    }

    void setDrive(float newDrive)
    {
        // Drive is -18 dB ... +18 dB
        drive = newDrive * 36.0f - 18.0f;

        // Convert dB to linear gain
        drive = std::pow(10.0f, drive * 0.05f);
    }

    void setOutput(float newOutput)
    {
        output = newOutput;
    }

    void setFrequencyRolloff(float f)
    {
        if (f < 0.0f) { f = 0.0f; }

        //TODO: don't hardcode sample rate here!
        coef = f * (2.0f * PI) / SAMPLE_RATE;
        if (coef > 1.0f) {
            coef = 1.0f;
        } else if (coef < 0.0f) {
            coef = 0.0f;
        }
    }

    void processInputSaturation(juce::AudioBuffer<float>& sampleBuffer, int numChannels)
    {
        //TODO: doesn't this allocate?
        // Create a copy of the input signal, allowing us to perform odd
        // and even harmonics distortion separately.
        sampleBufferCopy = sampleBuffer;

        // Odd harmonic distortion saturation
        processOddHarmonicWaveshaping(sampleBuffer, numChannels);
        sampleBuffer.applyGain(oddGain);

        // Even harmonic distortion saturation
        processEvenHarmonicWaveshaping(sampleBufferCopy, numChannels);
        sampleBufferCopy.applyGain(evenGain);

        // Mix the even and odd harmonic signals
        weightEvenAndOddWaveshaping(sampleBuffer, sampleBufferCopy, numChannels);

        // Apply single-pole lowpass filter, rolloff at 4000 Hz.
        lowPassFilter(sampleBuffer, numChannels);
    }

private:
    void processOddHarmonicWaveshaping(juce::AudioBuffer<float>& sampleBuffer, int numChannels)
    {
        // Note: In this plug-in, satThreshold is always 0.0f, drive is 1.0f,
        // output is 1.0f, and satRateOdd is 2.0f. So the following simplifies
        // to the formula: y = sign(x) * tanh(2.0 * abs(x)).

        for (int channel = 0; channel < numChannels; ++channel) {
            float* samples = sampleBuffer.getWritePointer(channel);

            for (int i = 0; i < sampleBuffer.getNumSamples(); ++i) {
                samples[i] *= drive;

                if (samples[i] > satThreshold) {
                    float t = std::tanh(satRateOdd * (std::abs(samples[i]) - satThreshold));
                    samples[i] = satThreshold + t * (1.0f - satThreshold);
                }
                else if (samples[i] < -satThreshold) {
                    float t = std::tanh(satRateOdd * (std::abs(samples[i]) - satThreshold));
                    samples[i] = -satThreshold - t * (1.0f - satThreshold);
                }

                samples[i] *= output;
            }
        }
    }

    void processEvenHarmonicWaveshaping(juce::AudioBuffer<float>& sampleBuffer, int numChannels)
    {
        for (int channel = 0; channel < numChannels; ++channel) {
            float* samples = sampleBuffer.getWritePointer(channel);

            for (int i = 0; i < sampleBuffer.getNumSamples(); ++i) {
                samples[i] = std::abs(samples[i]);
                samples[i] *= drive;
                samples[i] = std::tanh(satRateEven * samples[i]);
                samples[i] *= output;
            }
        }
    }

    void weightEvenAndOddWaveshaping(juce::AudioBuffer<float>& sampleBufferOdd,
                                     juce::AudioBuffer<float>& sampleBufferEven,
                                     int numChannels)
    {
        // This mixes the even harmonics into the buffer with the odd harmonics.
        // Not entirely sure why this applies oddGain and evenGain again, since
        // earlier we already did applyGain() on these buffers. oddGain is always
        // 1.0f in this plug-in, so that has no effect. evenGain is 0.3f, so this
        // just makes the even harmonics quieter.

        float g = 1.0f / (oddGain + evenGain);
        float og = oddGain * g;
        float eg = evenGain * g;

        for (int channel = 0; channel < numChannels; ++channel) {
            float* sampleOdd = sampleBufferOdd.getWritePointer(channel);
            float* sampleEven = sampleBufferEven.getWritePointer(channel);

            for (int i = 0; i < sampleBufferOdd.getNumSamples(); i++) {
                sampleOdd[i] = og * sampleOdd[i] + eg * sampleEven[i];
            }
        }
    }

    void lowPassFilter(juce::AudioBuffer<float>& sampleBuffer, int numChannels)
    {
        for (int channel = 0; channel < numChannels; ++channel) {
            float* sample = sampleBuffer.getWritePointer(channel);

            float last = priorSample[channel];
            float feedback = 1.0f - coef;

            for (int i = 0; i < sampleBuffer.getNumSamples(); ++i) {
                last = sample[i] = coef * sample[i] + feedback * last;
            }

            priorSample[channel] = last;
        }
    }

private:
    float evenGain;
    float oddGain;

    float satRateEven;
    float satRateOdd;    // How quickly the saturation approaches the upper bound.
    float satThreshold;  // The amplitude level at which the saturation waveshaping begins.

    float drive;
    float output;

    juce::AudioBuffer<float> sampleBufferCopy;

    // For single-pole low pass filter
    float priorSample[10];   // assume max. 10 channels
    float coef;
};
