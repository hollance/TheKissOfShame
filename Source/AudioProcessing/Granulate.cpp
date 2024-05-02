/***************************************************/
/*! \class Granulate
    \brief STK granular synthesis class.

    This class implements a real-time granular synthesis algorithm
    that operates on an input soundfile.  Multi-channel files are
    supported.  Various functions are provided to allow control over
    voice and grain parameters.

    The functionality of this class is based on the program MacPod by
    Chris Rolfe and Damian Keller, though there are likely to be a
    number of differences in the actual implementation.

    by Gary Scavone, 2005 - 2010.
*/
/***************************************************/

#include "Granulate.h"
#include <cmath>

Granulate::Granulate(unsigned int nVoices, const char* data, int size)
{
    rng.setSeedRandomly();

    setGrainParameters();  // use default values
    setRandomFactor();
    gStretch_ = 0;
    stretchCounter_ = 0;

    audioData.reset(loadSampleFromMemory(data, size));

    int numChannels = audioData->getNumChannels();
    for (int i = 0; i < numChannels; ++i) {
        lastFrame_.push_back(0.0f);
    }

    reset();
    setVoices(nVoices);
}

void Granulate::setStretch(unsigned int stretchFactor)
{
    if (stretchFactor <= 1)
        gStretch_ = 0;
    else if (gStretch_ >= 1000)
        gStretch_ = 1000;
    else
        gStretch_ = stretchFactor - 1;
}

void Granulate::setGrainParameters(unsigned int duration, unsigned int rampPercent,
                                   int offset, unsigned int delay)
{
    gDuration_ = duration;
    if (gDuration_ == 0) {
        gDuration_ = 1;
    }

    gRampPercent_ = rampPercent;
    if (gRampPercent_ > 100) {
        gRampPercent_ = 100;
    }

    gOffset_ = offset;
    gDelay_ = delay;
}

void Granulate::setRandomFactor(float randomness)
{
    if (randomness < 0.0f)
        gRandomFactor_ = 0.0f;
    else if (randomness > 1.0f)
        gRandomFactor_ = 0.97f;
    else
        gRandomFactor_ = 0.97f * randomness;
}

void Granulate::reset()
{
    gPointer_ = 0.0f;

    // Reset grain parameters.
    size_t count;
    size_t nVoices = grains_.size();
    for (unsigned int i = 0; i < grains_.size(); ++i) {
        grains_[i].repeats = 0;
        count = size_t(i * gDuration_ * 0.001f * SAMPLE_RATE / nVoices);
        grains_[i].counter = (unsigned long)count;
        grains_[i].state = GRAIN_STOPPED;
    }

    for (size_t i = 0; i < lastFrame_.size(); ++i) {
        lastFrame_[i] = 0.0f;
    }
}

void Granulate::setVoices(unsigned int nVoices)
{
    size_t oldSize = grains_.size();
    grains_.resize(nVoices);

    // Initialize new grain voices.
    size_t count;
    for (size_t i = oldSize; i < nVoices; i++) {
        grains_[i].repeats = 0;
        count = size_t(i * gDuration_ * 0.001f * SAMPLE_RATE / nVoices);
        grains_[i].counter = (unsigned long)count;
        grains_[i].pointer = gPointer_;
        grains_[i].state = GRAIN_STOPPED;
    }

    gain_ = 1.0f / grains_.size();
}

void Granulate::calculateGrain(Granulate::Grain& grain)
{
    if (grain.repeats > 0) {
        grain.repeats--;
        grain.pointer = float(grain.startPointer);
        if (grain.attackCount > 0) {
            grain.eScaler = 0.0;
            grain.eRate = -grain.eRate;
            grain.counter = grain.attackCount;
            grain.state = GRAIN_FADEIN;
        } else {
            grain.counter = grain.sustainCount;
            grain.state = GRAIN_SUSTAIN;
        }
        return;
    }

    // Calculate duration and envelope parameters.
    float seconds = gDuration_ * 0.001f;
    seconds += seconds * gRandomFactor_ * (rng.nextFloat() * 2.0f - 1.0f);
    unsigned long count = (unsigned long)(seconds * SAMPLE_RATE);
    grain.attackCount = (unsigned int)(gRampPercent_ * 0.005f * count);
    grain.decayCount = grain.attackCount;
    grain.sustainCount = count - 2 * grain.attackCount;
    grain.eScaler = 0.0;
    if (grain.attackCount > 0) {
        grain.eRate = 1.0f / grain.attackCount;
        grain.counter = grain.attackCount;
        grain.state = GRAIN_FADEIN;
    } else {
        grain.counter = grain.sustainCount;
        grain.state = GRAIN_SUSTAIN;
    }

    // Calculate delay parameter.
    seconds = gDelay_ * 0.001f;
    seconds += seconds * gRandomFactor_ * (rng.nextFloat() * 2.0f - 1.0f);
    count = (unsigned long)(seconds * SAMPLE_RATE);
    grain.delayCount = count;

    // Save stretch parameter.
    grain.repeats = gStretch_;

    // Calculate offset parameter.
    seconds = gOffset_ * 0.001f;
    seconds += seconds * gRandomFactor_ * rng.nextFloat();
    int offset = int(seconds * SAMPLE_RATE);

    // Add some randomization to the pointer start position.
    seconds = gDuration_ * 0.001f * gRandomFactor_ * (rng.nextFloat() * 2.0f - 1.0f);
    offset += int(seconds * SAMPLE_RATE);
    grain.pointer += offset;
    while (grain.pointer >= audioData->getNumSamples()) {
        grain.pointer -= audioData->getNumSamples();
    }

    if (grain.pointer < 0.0f) { grain.pointer = 0.0f; }
    grain.startPointer = (unsigned long)grain.pointer;
}

float Granulate::tick(unsigned int channel)
{
    unsigned int i, j;
    unsigned int nChannels = (unsigned int)lastFrame_.size();

    for (j = 0; j < nChannels; j++) {
        lastFrame_[j] = 0.0f;
    }

    if (audioData->getNumSamples() == 0) { return 0.0f; }

    float sample;
    for (i = 0; i < grains_.size(); i++) {

        if (grains_[i].counter == 0) {  // Update the grain state.

            switch (grains_[i].state) {

                case GRAIN_STOPPED:
                    // We're done waiting between grains ... setup for new grain
                    calculateGrain(grains_[i]);
                    break;

                case GRAIN_FADEIN:
                    // We're done ramping up the envelope
                    if (grains_[i].sustainCount > 0) {
                        grains_[i].counter = grains_[i].sustainCount;
                        grains_[i].state = GRAIN_SUSTAIN;
                        break;
                    }
                    // else no sustain state (i.e. perfect triangle window)
                    [[fallthrough]];

                case GRAIN_SUSTAIN:
                    // We're done with flat part of envelope ... setup to ramp down
                    if (grains_[i].decayCount > 0) {
                        grains_[i].counter = grains_[i].decayCount;
                        grains_[i].eRate = -grains_[i].eRate;
                        grains_[i].state = GRAIN_FADEOUT;
                        break;
                    }
                    // else no fade out state (gRampPercent = 0)
                    [[fallthrough]];

                case GRAIN_FADEOUT:
                    // We're done ramping down ... setup for wait between grains
                    if (grains_[i].delayCount > 0) {
                        grains_[i].counter = grains_[i].delayCount;
                        grains_[i].state = GRAIN_STOPPED;
                        break;
                    }
                    // else no delay (gDelay = 0)
                    calculateGrain(grains_[i]);
            }
        }

        // Accumulate the grain outputs.
        if (grains_[i].state > 0) {
            for (j = 0; j < nChannels; j++) {
                sample = *(audioData->getReadPointer(int(j), int(grains_[i].pointer)));

                if (grains_[i].state == GRAIN_FADEIN || grains_[i].state == GRAIN_FADEOUT) {
                    sample *= grains_[i].eScaler;
                    grains_[i].eScaler += grains_[i].eRate;
                }
                lastFrame_[j] += sample;
            }

            // Increment and check pointer limits.
            grains_[i].pointer++;
            if (grains_[i].pointer >= audioData->getNumSamples()) {
                grains_[i].pointer = 0.0f;
            }
        }

        // Decrement counter for all states.
        grains_[i].counter--;
    }

    // Increment our global file pointer at the stretch rate.
    if (stretchCounter_++ == gStretch_) {
        gPointer_++;
        if (gPointer_ >= audioData->getNumSamples()) {
            gPointer_ = 0.0f;
        }
        stretchCounter_ = 0;
    }

    return lastFrame_[channel];
}
