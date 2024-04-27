#include "PluginProcessor.h"
#include "PluginEditor.h"

KissOfShameAudioProcessor::KissOfShameAudioProcessor() :
    AudioProcessor(
        BusesProperties()
           .withInput("Input", juce::AudioChannelSet::stereo(), true)
           .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    ),
    params(apvts),
    audioGraph(params)
{
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

const juce::String KissOfShameAudioProcessor::getProgramName(int)
{
    return {};
}

void KissOfShameAudioProcessor::changeProgramName(int, const juce::String&)
{
}

bool KissOfShameAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto mono = juce::AudioChannelSet::mono();
    const auto stereo = juce::AudioChannelSet::stereo();
    const auto mainIn = layouts.getMainInputChannelSet();
    const auto mainOut = layouts.getMainOutputChannelSet();

    if (mainIn == mono && mainOut == mono) { return true; }
    if (mainIn == stereo && mainOut == stereo) { return true; }

    return false;
}

void KissOfShameAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    params.prepareToPlay(sampleRate);
    params.reset();

    audioGraph.prepareToPlay(sampleRate, samplesPerBlock);

    curRMSL = 0.0f;
    curRMSR = 0.0f;

    curPositionInfo.resetToDefault();

    playHeadPos = 0;
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

    params.update();

    playHeadPos = (playHeadPos + 1) % 99999;

    // Audio processing...
    audioGraph.processGraph(buffer, totalNumInputChannels);

    // Need to send the RMS below to the animation components for VU meters.
    // Measuring RMS over the buffer is simple but depends on the buffer size.
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

    // TODO: use up-to-date JUCE API for this
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
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void KissOfShameAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

juce::AudioProcessorParameter* KissOfShameAudioProcessor::getBypassParameter() const
{
    return params.bypassParam;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KissOfShameAudioProcessor();
}
