#pragma once

#include <JuceHeader.h>
#include "AudioProcessing/AudioGraph.h"

class KissOfShameAudioProcessor : public juce::AudioProcessor
{
public:
    KissOfShameAudioProcessor();
    ~KissOfShameAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

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

    //TODO: these are deprecated for managing the parameters!
    int getNumParameters() override;
    float getParameter(int index) override;
    void setParameter(int index, float newValue) override;
    const juce::String getParameterName(int index) override;
    const juce::String getParameterText(int index) override;

//TODO: these functions exist but aren't used, prefer this over direct variable access
//    float getCurrentRMSL() const noexcept { return curRMSL; }
//    float getCurrentRMSR() const noexcept { return curRMSR; }

    // All of these values get communicated to the host:
    enum AudioInputParameters
    {
        inputSaturationParam = 0,
        shameParam,
        hissParam,
        blendParam,
        bypassParam,
        outputParam,
        flangeParam,
        hurricaneSandyParam,
        totalNumParams,
    };

    float inputSaturation, shame, hiss, blend, output, flange;
    bool masterBypass;

    // TODO: this type is deprecated
    juce::AudioPlayHead::CurrentPositionInfo curPositionInfo;

    AudioGraph audioGraph;

    // TODO: make these atomic
    float curRMSL, curRMSR;

    int playHeadPos;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KissOfShameAudioProcessor)
};
