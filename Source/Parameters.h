#pragma once

#include <JuceHeader.h>

namespace ParameterID
{
    const juce::ParameterID bypass { "bypass", 1 };

//TODO: add these parameters
//    inputLevel = 1.0f;
//    shame = 0.0f;
//    age = 0.0f;
//    hiss = 0.0f;
//    blend = 0.0f;
//    outputLevel = 0.0f;
//    flange = 0.0f;
//    currentEnvironment
//    tapeType
//    printThrough
//    showReels
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

    juce::AudioParameterBool* bypassParam;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameters)
};
