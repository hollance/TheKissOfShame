#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts, 
                          const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);  // parameter does not exist or wrong type
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, ParameterID::bypass, bypassParam);
    castParameter(apvts, ParameterID::showReels, showReelsParam);
    castParameter(apvts, ParameterID::environment, environmentParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterBool>(
        ParameterID::bypass, "Bypass", false));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        ParameterID::showReels, "Show Reels", true));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        ParameterID::environment,
        "Environment",
        juce::StringArray {
            "Environment",
            "Off",
            "Studio Closet",
            "Humid Cellar",
            "Hot Locker",
            "Hurricane Sandy",
        },
        0));

    return layout;
}

void Parameters::prepareToPlay(double /*sampleRate*/) noexcept
{
}

void Parameters::reset() noexcept
{
}

void Parameters::update() noexcept
{
    bypassed = bypassParam->get();
    showReels = showReelsParam->get();
    environment = EShameEnvironments(environmentParam->getIndex());
}

void Parameters::smoothen() noexcept
{
}
