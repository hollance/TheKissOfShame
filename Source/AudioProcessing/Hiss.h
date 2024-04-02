#pragma once

#include "../shameConfig.h"

class Hiss
{
public:
    Hiss()
    {
        // Load the hiss waveform from the binary data.
        hissBuffer.reset(loadSampleFromMemory(BinaryData::Hiss_wav, BinaryData::Hiss_wavSize));

        // We expect this to be a stereo buffer.
        jassert(hissBuffer->getNumChannels() == 2);
        jassert(hissBuffer->getNumSamples() > 0);

        // Note that Hiss.wav is a 44.1 kHz sample. At 48 kHz it still sounds
        // like noise, but at higher rates a tone becomes audible.
        // For consistent results, it would be better to add a prepareToPlay()
        // method to this class and resample the hissBuffer if necessary.

        setHissLevel(0.0f);
    }

    void reset() noexcept
    {
        indx1 = 0;
        indx2 = hissBuffer->getNumSamples() / 2;
    }

    void processHiss(juce::AudioBuffer<float>& sampleBuffer, int numChannels) noexcept
    {
        const int hissLength = hissBuffer->getNumSamples();
        const int hissHalfway = hissLength / 2;

        for (int i = 0; i < sampleBuffer.getNumSamples(); ++i) {
            // The hiss waveform fades in at the beginning and out at the end.
            // To prevent these tapers from being audible and get a seamless
            // loop, we'll read the waveform at two locations that are half a
            // waveform length apart and blend between these two samples.
            float rampValue;
            if (indx1 < hissHalfway)
                rampValue = float(indx1) / float(hissHalfway);
            else
                rampValue = float(hissLength - indx1) / float(hissHalfway);

            //TODO: do this more efficiently by changing the order
            for (int channel = 0; channel < numChannels; ++channel) {
                float* samples = sampleBuffer.getWritePointer(channel);
                const float* readPtr = hissBuffer->getReadPointer(channel);

                // Blend between the two hiss samples.
                float hissSample = rampValue*readPtr[indx1] + (1.0f - rampValue)*readPtr[indx2];

                // Blend between the incoming signal and the hiss.
                samples[i] = signalLevel*samples[i] + hissLevel*hissSample;
            }

            indx1 = (indx1 + 1) % hissLength;
            indx2 = (indx2 + 1) % hissLength;
        }
    }

    void setAudioFile(const juce::String& audioFilePath)
    {
        juce::File hissFile(audioFilePath);
        if (hissFile.existsAsFile()) {
            hissBuffer.reset(loadSampleFromFile(hissFile));
        }
    }

    void setHissLevel(float level) noexcept
    {
        // TODO: maybe boost the max hiss level a bit?
        // Add up to -46 dB of hiss, reduce the dry level by the same amount.
        hissLevel = level * 0.005f;
        signalLevel = 1.0f - hissLevel;
    }

private:
    std::unique_ptr<juce::AudioBuffer<float>> hissBuffer;

    int indx1;
    int indx2;
    float hissLevel;
    float signalLevel;
};
