#pragma once

#include "../shameConfig.h"

#define BUFFER_SIZE (44100)

class Shame
{
public:
    Shame(int numChannels)
    {
        depth = 0.5f;
        rate = 2.0f;
        randPeriodicity = 0.0f;
        waveformIndx = 0.0f;

        // TODO put this in a reset() function
        curPos = 0;
        playPosition = 0.0f;
        curPos_wTable = 0.0f;
        rateFluctuation = 0.0f;

        // TODO: do this in a prepareToPlay() function to support other sample rates
        //       then we also don't need to pass numChannels to the constructor
        shameSampleBuffer.reset(new juce::AudioBuffer<float>(numChannels, BUFFER_SIZE));

        importWaveTables();
    }

    void processShame(juce::AudioBuffer<float>& sampleBuffer, int numChannels)
    {
        for (int i = 0; i < sampleBuffer.getNumSamples(); ++i) {

            // Push the input sample into the delay line. This is a circular
            // buffer that can store enough samples for the max modulation depth.
            // TODO: doing the loops this way is a bit inefficient
            for (int channel = 0; channel < numChannels; ++channel) {
                float curSample = sampleBuffer.getReadPointer(channel)[i];
                shameSampleBuffer->getWritePointer(channel)[curPos] = curSample;
            }

            // Calculate sample indices for linear interpolation.
            float fraction = playPosition - int(playPosition);
            int prevX = int(playPosition);
            int nextX = (prevX + 1) % BUFFER_SIZE;

            // Read from the delay line.
            for (int channel = 0; channel < numChannels; ++channel) {
                float* outSamples = sampleBuffer.getWritePointer(channel);
                const float* readPtr = shameSampleBuffer->getReadPointer(channel);
                outSamples[i] = linearInterpolate(readPtr[prevX], readPtr[nextX], fraction);
            }

            // Calculate the read position and wrap around if necessary.
            // processWavetable outputs a unipolar cosine (always negative,
            // otherwise we'd attempt to read into the future). This is used
            // as an LFO to modulate the read position in the delay line.
            // The rate of the LFO depends on the Shame knob setting and is
            // randomly varied after each cycle.
            playPosition = float(curPos) + depth*processWavetable();

            if (playPosition >= BUFFER_SIZE) {
                playPosition -= BUFFER_SIZE;
            }
            if (playPosition < 0) {
                playPosition += BUFFER_SIZE;
            }

            curPos = (curPos + 1) % BUFFER_SIZE;
        }
    }

    void setInterpolatedParameters(float input)
    {
        if (input < 0.0f) { input = 0.0f; }
        if (input > 1.0f) { input = 1.0f; }

        // Knob 0% to 50% sets depth 0 - 5. Periodicity and rate are fixed.
        if (input <= 0.5f) {
            depth = 5.0f * input / 0.5f;
            randPeriodicity = 0.5f;
            rate = 7.0f;
            waveformIndx = 0.0f;
        }
        // From 50% to 85% is depth 5 - 30, periodicity 0.5 down to 0.25,
        // rate 7.0 to 77.0.
        else if (input > 0.5f && input <= 0.85f) {
            depth = 5.0f + 25.0f*(input - 0.5f)/(0.85f - 0.5f);
            randPeriodicity = 0.5f - 0.25f*(input - 0.5f)/(0.85f - 0.5f);
            rate = 7.0f + 70.0f*(input - 0.5f)/(0.85f - 0.5f);
            waveformIndx = 0.0f;
        }
        // From 85% to 100% is depth 30 - 60, periodicity 0.25 to 0.75,
        // rate 77.0 down to 57.0.
        else if (input > 0.85f && input <= 1.0f) {
            depth = 30.0f + 30.0f*(input - 0.85f)/0.15f;
            randPeriodicity = 0.25f + 0.5f*(input - 0.85f)/0.15f;
            rate = 77.0f - 20.0f*(input - 0.85f)/0.15f;
            waveformIndx = 0.0f;
        }

        // TODO: Some of these values depend on the sample rate!
    }

private:
    void importWaveTables()
    {
        for (int i = 0; i < 4; ++i) {
            juce::AudioBuffer<float>* tempAudioBuffer = new juce::AudioBuffer<float>(1, BUFFER_SIZE);
            float* writePtr = tempAudioBuffer->getWritePointer(0);

            // Create a wavetable consisting of single cycle cosine waveform.
            // The amplitude of the cosine goes between 0.0 and -1.0.
            // TODO: Why do all four wavetables contain the exact same thing?
            // Not much point in interpolating between wavetables then.
            for (int j = 0; j < tempAudioBuffer->getNumSamples(); ++j) {
                double cosDomain = 2.0*PI*double(j) / double(BUFFER_SIZE - 1);
                writePtr[j] = float(0.5 * (std::cos(cosDomain) - 1.0));
            }

            waveTableBuffers.add(tempAudioBuffer);
        }
    }

    float linearInterpolate(float prevSamp, float nextSamp, float fraction)
    {
        return prevSamp*(1.0f - fraction) + nextSamp*fraction;
    }

    float processWavetable()
    {
        // Which two wavetables to interpolate between?
        int prevWaveIndx = 0;
        int nextWaveIndx = 0;
        if (waveformIndx < 0.0f) {
            prevWaveIndx = 0;
            nextWaveIndx = 0;
            waveformIndx = 0.0f;
        } else if (waveformIndx >= 0.0f && waveformIndx < 1.0f) {
            prevWaveIndx = 0;
            nextWaveIndx = 1;
        } else if (waveformIndx >= 1.0f && waveformIndx < 2.0f) {
            prevWaveIndx = 1;
            nextWaveIndx = 2;
        } else if (waveformIndx >= 2.0f && waveformIndx < 3.0f) {
            prevWaveIndx = 2;
            nextWaveIndx = 3;
        } else if (waveformIndx >= 3.0f) {
            prevWaveIndx = 3;
            nextWaveIndx = 3;
            waveformIndx = 3.0f;
        }

        // Blend factor for interpolating between the wavetables.
        float fracWaveIndx = waveformIndx - int(waveformIndx);

        // Get pointers for reading from both wavetables.
        const float* prevPtr = waveTableBuffers[prevWaveIndx]->getReadPointer(0);
        const float* nextPtr = waveTableBuffers[nextWaveIndx]->getReadPointer(0);

        // For interpolating between the samples in each wavetable.
        float fracPos = curPos_wTable - int(curPos_wTable);
        int prevPos = int(curPos_wTable);
        int nextPos = (prevPos + 1) % BUFFER_SIZE;

        // Read samples from each wavetable using linear interpolation.
        float w1Sample = prevPtr[prevPos]*(1.0f - fracPos) + prevPtr[nextPos]*fracPos;
        float w2Sample = nextPtr[prevPos]*(1.0f - fracPos) + nextPtr[nextPos]*fracPos;

        // Blend the samples from the two wavetables.
        float outsample = w1Sample*(1.0f - fracWaveIndx) + w2Sample*fracWaveIndx;

        // Increment the read position.
        curPos_wTable = curPos_wTable + rate + rateFluctuation;

        // Reached the end of the wavetable? Choose a new random rate fluctuation.
        if (curPos_wTable >= BUFFER_SIZE) {
            float random = float(rand() % 2000)/1000.0f - 1.0f;  // -1 ... +1
            rateFluctuation = random * rate * randPeriodicity;
            curPos_wTable -= BUFFER_SIZE;
        }
        if (curPos_wTable < 0) {
            curPos_wTable += BUFFER_SIZE;
        }

        return outsample;
    }

private:
    // Circular buffer for a delay line
    std::unique_ptr<juce::AudioBuffer<float>> shameSampleBuffer;

    int curPos;              // write position in the delay line
    float playPosition;      // read position in the delay line

    float depth;             // value between 0 and 60 samples
    float rate;              // how fast to step through the wavetable
    float rateFluctuation;   // random modification of the rate
    float randPeriodicity;   // used to calculate randFluctation

    // Several wavetables that each contain a single cycle waveform
    juce::OwnedArray<juce::AudioBuffer<float>> waveTableBuffers;

    float waveformIndx;      // which wavetable to use (can be fractional)
    float curPos_wTable;     // read position in the wavetable (fractional)
};
