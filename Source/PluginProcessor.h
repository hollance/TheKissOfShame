/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "AudioProcessing/AudioGraph.h"


//==============================================================================
/**
*/
class KissOfShameAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    KissOfShameAudioProcessor();
    ~KissOfShameAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
    
    float getCurrentRMSL(){return curRMSL;}
    float getCurrentRMSR(){return curRMSR;}
    
    
    //////All of these values get communicated to the host:
    enum AudioInputParameters
    {
        inputSaturationParam = 0,
        shameParam,
        hissParam,
        blendParam,
        bypassParam,
        outputParam,
        
        totalNumParams
    };
    
    float inputSaturation, shame, hiss, blend, output;
    bool masterBypass;
    
    AudioPlayHead::CurrentPositionInfo curPositionInfo;
    
    ScopedPointer<AudioGraph> aGraph;
    float curRMSL, curRMSR;

    int playHeadPos;
        
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KissOfShameAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
