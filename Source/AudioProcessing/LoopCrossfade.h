#pragma once

#include "../shameConfig.h"

class LoopCrossfade
{
public:
    LoopCrossfade()
    {
        // Load the loop waveform from the binary data.
        loopBuffer.reset(loadSampleFromMemory(BinaryData::LowLevelGrainNoise_wav,
                                              BinaryData::LowLevelGrainNoise_wavSize));

        // We expect this to be a stereo buffer.
        jassert(loopBuffer->getNumChannels() == 2);
        jassert(loopBuffer->getNumSamples() > 0);

        // For consistent results, it would be better to add a prepareToPlay()
        // method to this class and resample the loopBuffer if necessary.

        setLoopCrossfadeLevel(0.0f);

        // TODO: put into a reset() function
        indx1 = 0;
        indx2 = loopBuffer->getNumSamples() / 2;
    }

    void processLoopCrossfade(juce::AudioBuffer<float>& sampleBuffer, int numChannels) noexcept
    {
        const int loopLength = loopBuffer->getNumSamples();
        const int loopHalfway = loopLength / 2;

        for (int i = 0; i < sampleBuffer.getNumSamples(); ++i) {
            // To create a seamless transition between the end and beginning
            // of the loop, read the buffer at two locations that are half a
            // waveform length apart and blend between these two samples.
            float rampValue;
            if (indx1 < loopHalfway) {
                rampValue = float(indx1) / float(loopHalfway);
            } else {
                rampValue = float(loopLength - indx1) / float(loopHalfway);
            }

            //TODO: do this more efficiently by changing the order
            for (int channel = 0; channel < numChannels; ++channel) {
                float* samples = sampleBuffer.getWritePointer(channel);
                const float* readPtr = loopBuffer->getReadPointer(channel);

                // Blend between the two loop samples.
                float loopCrossfadeSample = rampValue*readPtr[indx1] + (1.0f - rampValue)*readPtr[indx2];

                // Blend between the incoming signal and the loop.
                samples[i] = signalLevel*samples[i] + loopLevel*loopCrossfadeSample;
            }

            indx1 = (indx1 + 1) % loopLength;
            indx2 = (indx2 + 1) % loopLength;
        }
    }

    float processLoopCrossSample(int channel) noexcept
    {
        const int loopLength = loopBuffer->getNumSamples();
        const int loopHalfway = loopLength / 2;

        float rampValue;
        if (indx1 < loopHalfway) {
            rampValue = float(indx1) / float(loopHalfway);
        } else {
            rampValue = float(loopLength - indx1) / float(loopHalfway);
        }

        const float* readPtr = loopBuffer->getReadPointer(channel);

        float loopCrossfadeSample = rampValue*readPtr[indx1] + (1.0f - rampValue)*readPtr[indx2];

        // Note: This code actually wouldn't work correctly with more than one
        // channel, since then it advances the index twice on each timestep.
        // In this plug-in it's used only with channel 0, even though the audio
        // file contains two channels.

        indx1 = (indx1 + 1) % loopLength;
        indx2 = (indx2 + 1) % loopLength;

        return loopLevel * loopCrossfadeSample;
    }

    void setAudioFile(const juce::String& audioFilePath)
    {
        juce::File audioFile(audioFilePath);
        if (audioFile.existsAsFile()) {
            loopBuffer.reset(loadSampleFromFile(audioFile));
        }
    }

    void setLoopCrossfadeLevel(float level) noexcept
    {
        loopLevel = level;
        signalLevel = 1.0f - loopLevel;
    }

private:
    std::unique_ptr<juce::AudioBuffer<float>> loopBuffer;

    int indx1;
    int indx2;

    float loopLevel;
    float signalLevel;
};
