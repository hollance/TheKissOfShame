#include "PluginProcessor.h"
#include "PluginEditor.h"

KissOfShameAudioProcessor::KissOfShameAudioProcessor() : masterBypass(false)
{
    // TODO: I think it might be a good idea to move this into prepareToPlay
    // but not sure yet how/when the parameters in AudioGraph are updated

    inputSaturation = 1.0f;
    shame = 0.0f;
    hiss = 0.0f;

    curRMSL = 0.0f;
    curRMSR = 0.0f;

    curPositionInfo.resetToDefault();

    playHeadPos = 0;
}

KissOfShameAudioProcessor::~KissOfShameAudioProcessor()
{
}

const juce::String KissOfShameAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int KissOfShameAudioProcessor::getNumParameters()
{
    return 0;
}

float KissOfShameAudioProcessor::getParameter(int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case inputSaturationParam:     return inputSaturation;
        case shameParam:               return shame;
        case hissParam:                return hiss;
        case blendParam:               return blend;
        case bypassParam:              return masterBypass;
        case outputParam:              return output;
        case flangeParam:              return flange;
        default:                       return 0.0f;
    }
    return 0.0f;
}

void KissOfShameAudioProcessor::setParameter(int index, float newValue)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case inputSaturationParam:     inputSaturation = newValue; break;
        case shameParam:               shame = newValue; break;
        case hissParam:                hiss = newValue;  break;
        case blendParam:               blend = newValue; break;
        case bypassParam:              masterBypass = newValue; break;
        case outputParam:              output = newValue; break;
        case flangeParam:              flange = newValue; break;
        default:                       break;
    }
}

const juce::String KissOfShameAudioProcessor::getParameterName(int index)
{
    return {};
}

const juce::String KissOfShameAudioProcessor::getParameterText(int index)
{
    return {};
}

bool KissOfShameAudioProcessor::acceptsMidi() const
{
    return false;
}

bool KissOfShameAudioProcessor::producesMidi() const
{
    return false;
}

bool KissOfShameAudioProcessor::isMidiEffect() const
{
    return false;
}

double KissOfShameAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KissOfShameAudioProcessor::getNumPrograms()
{
    return 1;
}

int KissOfShameAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KissOfShameAudioProcessor::setCurrentProgram(int)
{
}

const String KissOfShameAudioProcessor::getProgramName(int)
{
    return {};
}

void KissOfShameAudioProcessor::changeProgramName(int, const juce::String&)
{
}

void KissOfShameAudioProcessor::prepareToPlay(double, int)
{
}

void KissOfShameAudioProcessor::releaseResources()
{
    curPositionInfo.resetToDefault();
}

void KissOfShameAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    playHeadPos = (playHeadPos + 1) % 99999;

    // Audio processing...
    audioGraph.processGraph(buffer, totalNumInputChannels);

    // Need to send the RMS below to the animation components for VU meters...
    // use juce's messaging system to keep the audio thread safe.
    curRMSL = buffer.getRMSLevel(0, 0, numSamples);
    if (totalNumInputChannels > 1) {
        curRMSR = buffer.getRMSLevel(1, 0, numSamples);
    } else {
        curRMSR = curRMSL;
    }

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, as they may contain garbage.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, numSamples);
    }

    // ask the host for the current time so we can display it...
    if (getPlayHead() != nullptr && getPlayHead()->getCurrentPosition(curPositionInfo))
    {
        // Successfully got the current time from the host..
    }
    else
    {
        // If the host fails to fill-in the current time, we'll just clear it to a default..
        curPositionInfo.resetToDefault();
    }
}

bool KissOfShameAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* KissOfShameAudioProcessor::createEditor()
{
    return new KissOfShameAudioProcessorEditor(*this);
}

void KissOfShameAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KissOfShameAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KissOfShameAudioProcessor();
}
