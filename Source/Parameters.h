#pragma once

#include <JuceHeader.h>
#include "shameConfig.h"

namespace ParameterIDs
{
    const juce::ParameterID input { "input", 1 };
    const juce::ParameterID output { "output", 1 };
    const juce::ParameterID shame { "shame", 1 };
    const juce::ParameterID age { "age", 1 };
    const juce::ParameterID hiss { "hiss", 1 };
    const juce::ParameterID blend { "blend", 1 };
    const juce::ParameterID flange { "flange", 1 };
    const juce::ParameterID bypass { "bypass", 1 };
    const juce::ParameterID link { "link", 1 };
    const juce::ParameterID showReels { "showReels", 1 };
    const juce::ParameterID printThrough { "printThrough", 1 };
    const juce::ParameterID environment { "environment", 1 };
    const juce::ParameterID tapeType { "tapeType", 1 };
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

    float inputDrive = 0.0f;
    float outputLevel = 0.0f;
    float shame = 0.0f;
    float age = 0.0f;
    float hiss = 0.0f;
    float blend = 0.0f;
    float flange = 0.0f;
    bool bypassed = false;
    bool link = false;
    bool showReels = true;
    bool printThrough = false;
    EShameEnvironments environment = eEnvironmentOff;
    int tapeType = 0;

    juce::AudioParameterBool* bypassParam;
    juce::AudioParameterBool* showReelsParam;
    juce::AudioParameterChoice* environmentParam;
    juce::AudioParameterFloat* inputParam;
    juce::AudioParameterFloat* outputParam;
    juce::AudioParameterFloat* shameParam;
    juce::AudioParameterFloat* ageParam;
    juce::AudioParameterFloat* hissParam;
    juce::AudioParameterFloat* blendParam;
    juce::AudioParameterBool* linkParam;
    juce::AudioParameterFloat* flangeParam;
    juce::AudioParameterChoice* tapeTypeParam;
    juce::AudioParameterBool* printThroughParam;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameters)
};
