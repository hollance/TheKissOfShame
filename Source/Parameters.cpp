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
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterBool>(ParameterID::bypass, "Bypass", false));

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
}

void Parameters::smoothen() noexcept
{
}
