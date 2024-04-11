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
    castParameter(apvts, ParameterID::input, inputParam);
    castParameter(apvts, ParameterID::output, outputParam);
    castParameter(apvts, ParameterID::shame, shameParam);
    castParameter(apvts, ParameterID::age, ageParam);
    castParameter(apvts, ParameterID::hiss, hissParam);
    castParameter(apvts, ParameterID::blend, blendParam);
    castParameter(apvts, ParameterID::flange, flangeParam);
    castParameter(apvts, ParameterID::bypass, bypassParam);
    castParameter(apvts, ParameterID::link, linkParam);
    castParameter(apvts, ParameterID::showReels, showReelsParam);
    castParameter(apvts, ParameterID::printThrough, printThroughParam);
    castParameter(apvts, ParameterID::environment, environmentParam);
    castParameter(apvts, ParameterID::tapeType, tapeTypeParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::input, "Input", juce::NormalisableRange<float>(), 0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::output, "Output", juce::NormalisableRange<float>(), 0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::shame, "Shame", juce::NormalisableRange<float>(), 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::age, "Age", juce::NormalisableRange<float>(), 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::hiss, "Hiss", juce::NormalisableRange<float>(), 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::blend, "Blend", juce::NormalisableRange<float>(), 1.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterID::flange, "Flange", juce::NormalisableRange<float>(), 0.0f));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        ParameterID::bypass, "Bypass", false));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        ParameterID::link, "Link", false));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        ParameterID::showReels, "Show Reels", true));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        ParameterID::printThrough, "Print-Through", false));

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

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        ParameterID::tapeType,
        "Tape Type",
        juce::StringArray { "S-111", "A-456" },
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
    // Input and output are -18 dB ... +18 dB.
    inputDrive = juce::Decibels::decibelsToGain(inputParam->get() * 36.0f - 18.0f);
    outputLevel = juce::Decibels::decibelsToGain(outputParam->get() * 36.0f - 18.0f);

    shame = shameParam->get();
    age = ageParam->get();
    hiss = hissParam->get();
    blend = blendParam->get();
    flange = flangeParam->get();
    bypassed = bypassParam->get();
    link = linkParam->get();
    showReels = showReelsParam->get();
    printThrough = printThroughParam->get();
    environment = EShameEnvironments(environmentParam->getIndex());
    tapeType = tapeTypeParam->getIndex();
}

void Parameters::smoothen() noexcept
{
}
