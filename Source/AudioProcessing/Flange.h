#pragma once

#include "../shameConfig.h"

#define FLANGE_BUFF_SIZE 2000

class Flange
{
public:
    Flange(int numChannels)
    {
        // TODO put this in a reset() function
        curPos = 0;
        playPosition = 0.0;
        curDepth = 0.0;
        targetDepth = 0.0;

        // TODO: do this in a prepareToPlay() function to support other sample rates
        //       then we also don't need to pass numChannels to the constructor
        flangeSampleBuffer.reset(new juce::AudioBuffer<float>(numChannels, FLANGE_BUFF_SIZE));
    }

    void processFlange(juce::AudioBuffer<float>& sampleBuffer, int numChannels)
    {
        for (int i = 0; i < sampleBuffer.getNumSamples(); ++i) {

            // Push the input sample into the delay line. This is a circular
            // buffer that can store enough samples for the max flange depth.
            // TODO: doing the loops this way is a bit inefficient
            for (int channel = 0; channel < numChannels; ++channel) {
                float curSample = sampleBuffer.getReadPointer(channel)[i];
                flangeSampleBuffer->getWritePointer(channel)[curPos] = curSample;
            }

            // Calculate sample indices for linear interpolation.
            float fraction = playPosition - int(playPosition);
            int prevX = int(playPosition);
            int nextX = (prevX + 1) % FLANGE_BUFF_SIZE;

            // For cubic interpolation:
            //int prevX2 = prevX - 1;
            //if (prevX2 < 0) { prevX2 += FLANGE_BUFF_SIZE; }
            //int nextX2 = (nextX + 1) % FLANGE_BUFF_SIZE;

            // The output sample is the input sample plus a delayed sample.
            // Since the delay length is small, this creates a comb filtering
            // effect. Usually a flanger runs off an LFO, but in this plug-in,
            // the user must manually perform the flanging.
            for (int channel = 0; channel < numChannels; ++channel) {
                float* outSamples = sampleBuffer.getWritePointer(channel);
                const float* readPtr = flangeSampleBuffer->getReadPointer(channel);

                // Linear interpolation:
                float delayedSample = linearInterpolate(readPtr[prevX], readPtr[nextX], fraction);

                // Cubic interpolation:
                //float delayedSample = cubicInterpolate(readPtr[prevX2],readPtr[prevX],
                //                                       readPtr[nextX], readPtr[nextX2], fraction);

                outSamples[i] = 0.5f * (outSamples[i] + delayedSample);
            }

            // Smoothly transition the current flange depth towards
            // the target depth using a one-pole filter.
            // TODO: filter coef should be independent of sample rate
            if (std::abs(targetDepth - curDepth) < 0.01f) {
                curDepth = targetDepth;
            } else {
                curDepth += (targetDepth - curDepth) * 0.001f;
            }

            // Calculate the read position and wrap around if necessary.
            // The delay is given by curDepth, which can be a fractional
            // number of samples.
            playPosition = float(curPos) - curDepth;

            if (playPosition >= FLANGE_BUFF_SIZE) {
                playPosition -= FLANGE_BUFF_SIZE;
            }
            if (playPosition < 0) {
                playPosition += FLANGE_BUFF_SIZE;
            }

            curPos = (curPos + 1) % FLANGE_BUFF_SIZE;
        }
    }

    void setDepth(float depth)
    {
        // The input value `depth` will be a value between 0 and 1.
        // The depth can vary between 0 and 1000 samples.
        // TODO: make this independent of sample rate
        targetDepth = depth * 1000.0f;
    }

private:
    float linearInterpolate(float prevSample, float nextSample, float fraction) const noexcept
    {
        return prevSample*(1.0f - fraction) + nextSample*fraction;
    }

    double cubicInterpolate(double prevSample2, double prevSample,
                            double nextSample, double nextSample2,
                            double fraction) const noexcept
    {
        double fracSqared = fraction*fraction;

        double a0 = -0.5*prevSample2 + 1.5*prevSample - 1.5*nextSample + 0.5*nextSample2;
        double a1 = prevSample2 - 2.5*prevSample + 2*nextSample - 0.5*nextSample2;
        double a2 = -0.5*prevSample2 + 0.5*nextSample;
        double a3 = prevSample;

        return a0*fraction*fracSqared + a1*fracSqared + a2*fraction + a3;
    }

private:
    // Circular buffer for a delay line
    std::unique_ptr<juce::AudioBuffer<float>> flangeSampleBuffer;

    int curPos;          // write position in the delay line
    float playPosition;  // read position in the delay line
    float curDepth;      // delay length
    float targetDepth;   // new delay length
};
