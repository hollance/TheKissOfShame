#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUIUtilities/BacklightComponent.h"
#include "GUIUtilities/CustomButton.h"
//#include "GUIUtilities/CustomKnob.h"
//#include "GUIUtilities/ImageAnimator.h"
#include "GUIUtilities/ImageInteractor.h"
//#include "GUIUtilities/EnvironmentsComponent.h"
//#include "GUIUtilities/ImageAnimationComponent.h"

class KissOfShameAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        public juce::Timer,
                                        public juce::Slider::Listener,
                                        public juce::Button::Listener,
                                        public juce::ActionListener
{
public:
    KissOfShameAudioProcessorEditor(KissOfShameAudioProcessor&);
    ~KissOfShameAudioProcessorEditor() override;

    void timerCallback() override;

    void sliderValueChanged(juce::Slider*) override;
    void buttonClicked(juce::Button* b) override;

    void mouseDoubleClick(const juce::MouseEvent &event) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

    void actionListenerCallback(const juce::String& message) override;

    void setReelMode(bool showReels);

    void initializeLevels();

//TODO: get rid of unique_ptrs

    //Images
    BacklightComponent backlight;
    ImageInteractor faceImage;

//    //Knobs
//    std::unique_ptr<CustomKnob> inputSaturationKnob;
//    std::unique_ptr<CustomKnob> shameKnob;
//    std::unique_ptr<CustomKnob> hissKnob;
//    std::unique_ptr<CustomKnob> blendKnob;
//    std::unique_ptr<CustomKnob> outputKnob;
//    std::unique_ptr<CustomKnob> ageKnob;

    // Buttons
    CustomButton bypassButton;
//    std::unique_ptr<CustomButton> tapeTypeButton;
//    std::unique_ptr<CustomButton> printThroughButton;
//    std::unique_ptr<CustomButton> linkIOButtonL;
//    std::unique_ptr<CustomButton> linkIOButtonR;

    // Used strictly to post messages for debugging
    juce::Label debugLabel;

//    //Components
//    std::unique_ptr<EnvironmentsComponent> environmentsComponent;
//
//    //animation
//    std::unique_ptr<ImageAnimationComponent> reelAnimation;
//    std::unique_ptr<ImageInteractor> vuMeterL;
//    std::unique_ptr<ImageInteractor> vuMeterR;
//    std::unique_ptr<ImageInteractor> shameKnobImage;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    KissOfShameAudioProcessor& audioProcessor;

    //TODO: make these parameters too so they get serialized with the APVTS
    bool showReels;
    bool linkIOMode;

    int priorProcessorTime;

    bool ignoreCallbacks = false;

    void setBypassButtonValue(float newValue);

    juce::ParameterAttachment bypassButtonAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KissOfShameAudioProcessorEditor)
};
