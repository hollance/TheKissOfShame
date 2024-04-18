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
    castParameter(apvts, ParameterIDs::input, inputParam);
    castParameter(apvts, ParameterIDs::output, outputParam);
    castParameter(apvts, ParameterIDs::shame, shameParam);
    castParameter(apvts, ParameterIDs::age, ageParam);
    castParameter(apvts, ParameterIDs::hiss, hissParam);
    castParameter(apvts, ParameterIDs::blend, blendParam);
    castParameter(apvts, ParameterIDs::flange, flangeParam);
    castParameter(apvts, ParameterIDs::bypass, bypassParam);
    castParameter(apvts, ParameterIDs::link, linkParam);
    castParameter(apvts, ParameterIDs::showReels, showReelsParam);
    castParameter(apvts, ParameterIDs::printThrough, printThroughParam);
    castParameter(apvts, ParameterIDs::environment, environmentParam);
    castParameter(apvts, ParameterIDs::tapeType, tapeTypeParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Input and Output are metaparameters because in Linked mode,
    // changing one will change the other.
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::input, "Input", juce::NormalisableRange<float>(), 0.5f,
        juce::AudioParameterFloatAttributes().withMeta(true)));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::output, "Output", juce::NormalisableRange<float>(), 0.5f,
        juce::AudioParameterFloatAttributes().withMeta(true)));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::shame, "Shame", juce::NormalisableRange<float>(), 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::age, "Age", juce::NormalisableRange<float>(), 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::hiss, "Hiss", juce::NormalisableRange<float>(), 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::blend, "Blend", juce::NormalisableRange<float>(), 1.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::flange, "Flange", juce::NormalisableRange<float>(), 0.0f));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        ParameterIDs::bypass, "Bypass", false));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        ParameterIDs::link, "Link", false));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        ParameterIDs::showReels, "Show Reels", true));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        ParameterIDs::printThrough, "Print-Through", false));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        ParameterIDs::environment,
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
        ParameterIDs::tapeType,
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
