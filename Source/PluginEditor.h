/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "GUIUtilities/CustomKnob.h"
#include "GUIUtilities/ImageAnimator.h"
#include "GUIUtilities/CustomButton.h"
#include "GUIUtilities/ImageInteractor.h"
#include "GUIUtilities/EnvironmentsComponent.h"

//==============================================================================
/**
*/
class KissOfShameAudioProcessorEditor  : public AudioProcessorEditor,
                                         public Timer,
                                         public SliderListener,
                                         public Button::Listener
{
public:
    KissOfShameAudioProcessorEditor (KissOfShameAudioProcessor* ownerFilter);
    ~KissOfShameAudioProcessorEditor();
    
    void timerCallback() override;
    
    void sliderValueChanged (Slider*) override;
    
    void buttonClicked (Button* b);
    virtual void timerCallback(int timerID){};
    
    void changeListenerCallback (ChangeBroadcaster *source){};
    
    //Images
    Image faceImage;
    
    //Knobs
    ScopedPointer<CustomKnob> inputSaturationKnob;
    ScopedPointer<CustomKnob> shameKnob;
    ScopedPointer<CustomKnob> hissKnob;
    ScopedPointer<CustomKnob> blendKnob;
    ScopedPointer<CustomKnob> outputKnob;
    ScopedPointer<CustomKnob> ageKnob;
    
    //buttons
    ScopedPointer<CustomButton> bypassButton;
    ScopedPointer<CustomButton> daysYearsButton;
    ScopedPointer<CustomButton> tapeTypeButton;
    ScopedPointer<CustomButton> printThroughButton;
    
    //labels
    Label debugLabel; //Used strictly to post messages for debugging...
    
    //Components
    ScopedPointer<EnvironmentsComponent> environmentsComponent;

    
    //animation
    ScopedPointer<ImageAnimator> reelAnimation;
    ScopedPointer<ImageInteractor> vuMeterL;
    ScopedPointer<ImageInteractor> vuMeterR;
    ScopedPointer<ImageInteractor> shameKnobImage;
    
    
private:
    
    //TODO: REMOVE!!! used in hack to control reel animation start/stop
    int prevProcessorIncr;
    
    KissOfShameAudioProcessor* getProcessor() const
    {
        return static_cast <KissOfShameAudioProcessor*> (getAudioProcessor());
    }
    
    


    //==============================================================================
    // This is just a standard Juce paint method...
    void paint (Graphics& g);
};


#endif  // PLUGINEDITOR_H_INCLUDED
