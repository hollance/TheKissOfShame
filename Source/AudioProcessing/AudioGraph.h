#pragma once

#include "../shameConfig.h"
#include "../Parameters.h"
#include "Hiss.h"
#include "Shame.h"
#include "InputSaturation.h"
#include "Flange.h"
#include "HurricaneSandy.h"
#include "Blend.h"

class AudioGraph
{
public:
    AudioGraph(Parameters& params_) : params(params_)
    {
        // do nothing
    }

    void prepareToPlay(float sampleRate, int /*samplesPerBlock*/)
    {
        // TODO: allocate audioGraphProcessingBuffer

        inSaturation.prepareToPlay(sampleRate);
        flange.prepareToPlay(sampleRate);
        hurricaneSandy.prepareToPlay(sampleRate);
        shame.prepareToPlay(sampleRate);
        hiss.prepareToPlay(sampleRate);
    }

    void reset() noexcept
    {
        inSaturation.reset();
        flange.reset();
        hurricaneSandy.reset();
        shame.reset();
        hiss.reset();

        // Force recalculation of these parameters.
        lastShame = -1.0f;
        lastAge = -1.0f;
    }

    void processGraph(juce::AudioBuffer<float>& audioBuffer, int numChannels)
    {
        // Update the parameters.
        hiss.setHissLevel(params.hiss);
        blend.setBlendLevel(params.blend);
        if (lastShame != params.shame) {
            shame.setInterpolatedParameters(params.shame);
            lastShame = params.shame;
        }
        if (lastAge != params.age) {
            hurricaneSandy.setInterpolatedParameters(params.age);
            lastAge = params.age;
        }
        flange.setDepth(params.flange);

        if (params.bypassed) return;

        // Apply the input drive. This is a simple linear gain.
        audioBuffer.applyGain(params.inputDrive);

        // Make a copy of the original audio to be used strictly for processing
        // TODO: doesn't this allocate memory?
        audioGraphProcessingBuffer = audioBuffer;

        ////////// Process Audio //////////

        // 1. Incoming audio gets flange and saturation processing.

        // Note that the saturation module has settings for threshold, drive,
        // etc, but they'e set to fixed values in this plug-in. The input gain
        // previously applied determines how saturated the wet signal becomes.
        // This saturation is always being applied.
        inSaturation.processInputSaturation(audioGraphProcessingBuffer, numChannels);

        flange.processFlange(audioGraphProcessingBuffer, numChannels);

        // 2. Add environment effects
        switch (params.environment)
        {
            case eEnvironmentEnvironment:
            case eEnvironmentOff:
            case eEnvironmentStudioCloset:
            case eEnvironmentHumidCellar:
            case eEnvironmentHotLocker:
                break;
            case eEnvironmentHurricaneSandy:
                hurricaneSandy.processHurricaneSandy(audioGraphProcessingBuffer, numChannels);
                break;
            default:
                break;
        }

        // 3. Add hiss and the shame feature
        hiss.processHiss(audioGraphProcessingBuffer, numChannels);
        shame.processShame(audioGraphProcessingBuffer, numChannels);

        // 4. Blend the processed audio with the original signal
        blend.processBlend(audioBuffer, audioGraphProcessingBuffer, numChannels);

        ////////// End Process Audio //////////

        // 5. Apply the final output level. Again a simple linear gain.
        audioBuffer.applyGain(params.outputLevel);
    }

private:
    Parameters& params;

    juce::AudioBuffer<float> audioGraphProcessingBuffer;

    InputSaturation inSaturation { 0.0f, 2.0f, 0.272f };
    Flange flange;
    HurricaneSandy hurricaneSandy;
    Shame shame;
    Hiss hiss;
    Blend blend;

    float lastShame;
    float lastAge;
};
