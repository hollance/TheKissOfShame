#pragma once

#include "../shameConfig.h"


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


// This code was taken from STK (https://github.com/thestk/stk) and modified to
// work with JUCE. Also tweaked it a bit for readability and to silence warnings.


class Granulate
{
public:
    //! Constructor taking input audio file and number of voices arguments.
    Granulate(unsigned int nVoices, const char* data, int size);

    //! Reset the file pointer and all existing grains to the file start.
    /*!
     Multiple grains are offset from one another in time by grain
     duration / nVoices.
     */
    void reset();

    //! Set the number of simultaneous grain "voices" to use.
    /*!
     Multiple grains are offset from one another in time by grain
     duration / nVoices.  For this reason, it is best to set the grain
     parameters before calling this function (during initialization).
     */
    void setVoices(unsigned int nVoices = 1);

    //! Set the stretch factor used for grain playback (1 - 1000).
    /*!
     Granular synthesis allows for time-stetching without affecting
     the original pitch of a sound.  A stretch factor of 4 will produce
     a resulting sound of length 4 times the orignal sound.  The
     default parameter of 1 produces no stretching.
     */
    void setStretch(unsigned int stretchFactor = 1);

    //! Set global grain parameters used to determine individual grain settings.
    /*!
     Each grain is defined as having a length of \e duration
     milliseconds which must be greater than zero.  For values of \e
     rampPercent (0 - 100) greater than zero, a linear envelope will be
     applied to each grain.  If \e rampPercent = 100, the resultant
     grain "window" is triangular while \e rampPercent = 50 produces a
     trapezoidal window.  In addition, each grain can have a time delay
     of length \e delay and a grain pointer increment of length \e
     offset, which can be negative, before the next ramp onset (in
     milliseconds).  The \e offset parameter controls grain pointer
     jumps between enveloped grain segments, while the \e delay
     parameter causes grain calculations to pause between grains.  The
     actual values calculated for each grain will be randomized by a
     factor set using the setRandomFactor() function.
     */
    void setGrainParameters(unsigned int duration = 30, unsigned int rampPercent = 50,
                            int offset = 0, unsigned int delay = 0);

    //! This factor is used when setting individual grain parameters (0.0 - 1.0).
    /*!
     This random factor is applied when all grain state durations
     are calculated.  If set to 0.0, no randomness occurs.  When
     randomness = 1.0, a grain segment of length \e duration will be
     randomly augmented by up to +- \e duration seconds (i.e., a 30
     millisecond length will be augmented by an extra length of up to
     +30 or -30 milliseconds).
     */
    void setRandomFactor(float randomness = 0.1f);

    //! Compute one sample frame and return the specified \c channel value.
    float tick(unsigned int channel = 0);

    enum GrainState
    {
        GRAIN_STOPPED,
        GRAIN_FADEIN,
        GRAIN_SUSTAIN,
        GRAIN_FADEOUT
    };

protected:
    struct Grain
    {
        float eScaler = 0.0f;
        float eRate = 0.0f;
        unsigned long attackCount = 0;
        unsigned long sustainCount = 0;
        unsigned long decayCount = 0;
        unsigned long delayCount = 0;
        unsigned long counter = 0;
        float pointer = 0.0f;
        unsigned long startPointer = 0;
        unsigned int repeats = 0;
        GrainState state = GRAIN_STOPPED;
    };

    void calculateGrain(Granulate::Grain& grain);

    std::vector<float> lastFrame_;
    std::unique_ptr<juce::AudioBuffer<float>> audioData;
    std::vector<Grain> grains_;
    float gPointer_;

    // Global grain parameters.
    unsigned int gDuration_;
    unsigned int gRampPercent_;
    unsigned int gDelay_;
    unsigned int gStretch_;
    unsigned int stretchCounter_;
    int gOffset_;
    float gRandomFactor_;
    float gain_;

    juce::Random rng;
};
