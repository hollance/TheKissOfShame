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
        shame.setInterpolatedParameters(0.0f);

        outputLevel = 1.0f;
        inputDrive = 1.0f;
    }

    void prepareToPlay(double /*sampleRate*/, int /*samplesPerBlock*/)
    {
        // TODO: allocate audioGraphProcessingBuffer

        flange.setDepth(0.0f);
        hiss.reset();
    }

    void processGraph(juce::AudioBuffer<float>& audioBuffer, int numChannels)
    {
        // Update the parameters. Could be optimized by only doing this when
        // the parameters have actually changed.
        flange.setDepth(params.flange);
        blend.setBlendLevel(params.blend);

        if (params.bypassed) return;

        // Apply the input drive. This is a simple linear gain.
        audioBuffer.applyGain(inputDrive);

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
        audioBuffer.applyGain(outputLevel);
    }

    void setInputDrive(float drive)
    {
        // Express input drive as -18 dB ... +18 dB
        inputDrive = drive * 36.0f - 18.0f;

        // Convert dB to linear gain
        inputDrive = std::pow(10.0f, inputDrive * 0.05f);
    }

    void setOutputLevel(float level)
    {
        // Express output level as -18 dB ... +18 dB
        outputLevel = level * 36.0f - 18.0f;

        // Convert dB to linear gain
        outputLevel = std::pow(10.0f, outputLevel * 0.05f);
    }

    float getOutputLevel() const noexcept
    {
        return outputLevel;
    }

    bool isGraphBypassed() const noexcept
    {
        return params.bypassed;
    }

    void setAudioUnitParameters(AUParameter param, float paramLevel)
    {
        // TODO: replace this with an APVTS and update these properties
        // from within processBlock

        switch (param) {
//TODO: these parameters are never used anywhere
//            case eSaturationDrive: inSaturation->setDrive(paramLevel); break;
//            case eSaturationOuput: inSaturation->setOutput(paramLevel); break;
//            case eSaturationRate: inSaturation->setRateOdd(paramLevel); break;
//            case eSaturationThresh: inSaturation->setThreshold(paramLevel); break;
//            case eSaturationGlobalLevel: inSaturation->setGlobalLevel(paramLevel); break;
//            case eShameFreq: shame->setRate(paramLevel); break;
//            case eShameDepth: shame->setDepth(paramLevel); break;

            case eShameGlobalLevel:
                shame.setInterpolatedParameters(paramLevel);
                break;
            case eHurricaneSandyGlobalLevel:
                hurricaneSandy.setInterpolatedParameters(paramLevel);
                break;

            case eHissLevel:   hiss.setHissLevel(paramLevel); break;
//            case eBlendLevel:  blend.setBlendLevel(paramLevel); break;
//            case eFlangeDepth: flange.setDepth(paramLevel); break;
//            case eBypass:      bypassGraph = (paramLevel >= 0.5f); break;
            case eInputDrive:  setInputDrive(paramLevel); break;
            case eOutputLevel: setOutputLevel(paramLevel); break;

            default: break;
        }
    }

private:
    Parameters& params;

    juce::AudioBuffer<float> audioGraphProcessingBuffer;

    InputSaturation inSaturation { 0.0f, 2.0f, 0.272f };
    Flange flange { 2 };
    HurricaneSandy hurricaneSandy;
    Shame shame { 2 };
    Hiss hiss;
    Blend blend;

    float inputDrive;
    float outputLevel;
};
