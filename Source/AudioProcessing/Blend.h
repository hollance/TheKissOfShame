#pragma once

#include "../shameConfig.h"

/**
  Simple dry/wet mix
 */
class Blend
{
public:
    Blend()
    {
        blendValue = 0.0f;
    }

    void processBlend(juce::AudioBuffer<float>& buffer1,
                      juce::AudioBuffer<float>& buffer2,
                      int numChannels) noexcept
    {
        const float blend1 = 1.0f - blendValue;
        const float blend2 = blendValue;

        for (int channel = 0; channel < numChannels; ++channel) {
            float* samples = buffer1.getWritePointer(channel);

            const float* readPtr1 = buffer1.getReadPointer(channel);
            const float* readPtr2 = buffer2.getReadPointer(channel);

            for (int i = 0; i < buffer1.getNumSamples(); ++i) {
                samples[i] = blend1 * readPtr1[i] + blend2 * readPtr2[i];
            }
        }
    }

    void setBlendLevel(float level) noexcept
    {
        blendValue = level;
    }

private:
    float blendValue;
};
