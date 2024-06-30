#pragma once

#include <JuceHeader.h>
#include "AudioProcessing/AudioGraph.h"
#include "Parameters.h"

class KissOfShameAudioProcessor : public juce::AudioProcessor
{
public:
    KissOfShameAudioProcessor();
    ~KissOfShameAudioProcessor() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorParameter* getBypassParameter() const override;

public:
    bool detectOverload(juce::AudioBuffer<float>& buffer);

    juce::AudioProcessorValueTreeState apvts {
        *this, nullptr, "Parameters", Parameters::createParameterLayout()
    };

    Parameters params;
    AudioGraph audioGraph;

    // TODO: make these atomic
    float curRMSL, curRMSR;

    // TODO: this type is deprecated
    juce::AudioPlayHead::CurrentPositionInfo curPositionInfo;

    int playHeadPos;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KissOfShameAudioProcessor)
};
