#pragma once

#include <JuceHeader.h>
#include "shameConfig.h"

namespace ParameterID
{
    const juce::ParameterID bypass { "bypass", 1 };
    const juce::ParameterID showReels { "showReels", 1 };
    const juce::ParameterID environment { "environment", 1 };

//TODO: add these parameters
//    inputLevel = 1.0f;
//    shame = 0.0f;
//    age = 0.0f;
//    hiss = 0.0f;
//    blend = 0.0f;
//    outputLevel = 0.0f;
//    flange = 0.0f;
//    tapeType
//    printThrough
//    linkInputOutput
}

class Parameters
{
public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void update() noexcept;
    void smoothen() noexcept;

    bool bypassed = false;
    bool showReels = true;
    EShameEnvironments environment = eEnvironmentOff;

    juce::AudioParameterBool* bypassParam;
    juce::AudioParameterBool* showReelsParam;
    juce::AudioParameterChoice* environmentParam;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameters)
};
