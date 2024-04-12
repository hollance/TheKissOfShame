#include "PluginProcessor.h"
#include "PluginEditor.h"

KissOfShameAudioProcessorEditor::KissOfShameAudioProcessorEditor(KissOfShameAudioProcessor& p) :
    AudioProcessorEditor(&p),
    audioProcessor(p),
    environmentsComponent(*p.params.environmentParam),
    reelAnimation(*p.params.flangeParam, 31, BinaryData::Wheels_png, BinaryData::Wheels_pngSize),
    priorProcessorTime(0),
    bypassButtonAttachment(
        *p.params.bypassParam, [this](float f){ setBypassButtonValue(f); }, nullptr
    ),
    showReelsAttachment(
        *p.params.showReelsParam, [this](float f){ setShowReelsValue(f); }, nullptr
    ),
    linkButtonAttachment(
        *p.params.linkParam, [this](float f){ setLinkButtonValue(f); }, nullptr
    )
{
    backlight.setTopLeftPosition(0, 703 - backlight.getHeight());
    addAndMakeVisible(backlight);

    faceImage.setNumFrames(1);
    faceImage.setDimensions(0, 0, 960, 703);
    faceImage.setAnimationImage(BinaryData::FaceWithReels_png, BinaryData::FaceWithReels_pngSize);
    faceImage.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(faceImage);

    ////////// COMPONENTS //////////

    environmentsComponent.setTopLeftPosition(388, 654);
    addAndMakeVisible(environmentsComponent);

    ////////// KNOBS //////////

    inputSaturationKnob.setKnobImage(BinaryData::InputKnob_png, BinaryData::InputKnob_pngSize);
    inputSaturationKnob.setNumFrames(65);
    inputSaturationKnob.setKnobDimensions(104, 521, 116, 116);
    inputSaturationKnob.addListener(this);
    addAndMakeVisible(inputSaturationKnob);

    shameKnobImage.setNumFrames(65);
    shameKnobImage.setDimensions(401, 491, 174, 163);
    shameKnobImage.setAnimationImage(BinaryData::ShameKnob_png, BinaryData::ShameKnob_pngSize);
    addAndMakeVisible(shameKnobImage);

    shameKnob.setKnobImage(BinaryData::ShameCross_png, BinaryData::ShameCross_pngSize);
    shameKnob.setNumFrames(65);
    shameKnob.setKnobDimensions(401, 491, 174, 163);
    shameKnob.addListener(this);
    addAndMakeVisible(shameKnob);

    hissKnob.setKnobImage(BinaryData::HissKnob_png, BinaryData::HissKnob_pngSize);
    hissKnob.setNumFrames(65);
    hissKnob.setKnobDimensions(547, 455, 78, 72);
    addAndMakeVisible(hissKnob);

    blendKnob.setKnobImage(BinaryData::BlendKnob_png, BinaryData::BlendKnob_pngSize);
    blendKnob.setNumFrames(65);
    blendKnob.setKnobDimensions(705, 455, 78, 72);
    addAndMakeVisible(blendKnob);

    outputKnob.setKnobImage(BinaryData::OutputKnob_png, BinaryData::OutputKnob_pngSize);
    outputKnob.setNumFrames(65);
    outputKnob.setKnobDimensions(757, 521, 122, 116);
    outputKnob.addListener(this);
    addAndMakeVisible(outputKnob);

    ageKnob.setKnobImage(BinaryData::AgeKnob_png, BinaryData::AgeKnob_pngSize);
    ageKnob.setNumFrames(65);
    ageKnob.setKnobDimensions(350, 455, 74, 72);
    addAndMakeVisible(ageKnob);

    ////////// BUTTONS //////////

    bypassButton.setTopLeftPosition(202, 469);
    bypassButton.setClippedCustomOnImage(BinaryData::Bypass_png, BinaryData::Bypass_pngSize, 0, 68, 34, 34);
    bypassButton.setClippedCustomOffImage(BinaryData::Bypass_png, BinaryData::Bypass_pngSize, 0, 0, 34, 34);
    bypassButton.setClickingTogglesState(true);
    addAndMakeVisible(bypassButton);

    bypassButtonAttachment.sendInitialUpdate();
    bypassButton.addListener(this);

    tapeTypeButton.setTopLeftPosition(233, 610);
    tapeTypeButton.setClippedCustomOnImage(BinaryData::TapeType_png, BinaryData::TapeType_pngSize, 0, 0, 42, 39);
    tapeTypeButton.setClippedCustomOffImage(BinaryData::TapeType_png, BinaryData::TapeType_pngSize, 0, 39, 42, 39);
    tapeTypeButton.setClickingTogglesState(true);
    addAndMakeVisible(tapeTypeButton);

    printThroughButton.setTopLeftPosition(698, 609);
    printThroughButton.setClippedCustomOnImage(BinaryData::PrintThrough_png, BinaryData::PrintThrough_pngSize, 0, 41, 47, 41);
    printThroughButton.setClippedCustomOffImage(BinaryData::PrintThrough_png, BinaryData::PrintThrough_pngSize, 0, 0, 47, 41);
    printThroughButton.setClickingTogglesState(true);
    addAndMakeVisible(printThroughButton);

    linkIOButtonL.setTopLeftPosition(137, 605);
    linkIOButtonL.setClippedCustomOnImage(BinaryData::Link_png, BinaryData::Link_pngSize, 0, 0, 50, 50);
    linkIOButtonL.setClippedCustomOffImage(BinaryData::Link_png, BinaryData::Link_pngSize, 0, 0, 50, 50);
    linkIOButtonL.resizeButton(0.6f);
    linkIOButtonL.setClickingTogglesState(true);
    addAndMakeVisible(linkIOButtonL);

    linkIOButtonR.setTopLeftPosition(792, 605);
    linkIOButtonR.setClippedCustomOnImage(BinaryData::Link_png, BinaryData::Link_pngSize, 0, 0, 50, 50);
    linkIOButtonR.setClippedCustomOffImage(BinaryData::Link_png, BinaryData::Link_pngSize, 0, 0, 50, 50);
    linkIOButtonR.resizeButton(0.6f);
    linkIOButtonR.setClickingTogglesState(true);
    addAndMakeVisible(linkIOButtonR);

    linkButtonAttachment.sendInitialUpdate();
    linkIOButtonL.addListener(this);
    linkIOButtonR.addListener(this);

    ////////// Animation //////////

    reelAnimation.setFrameDimensions(0, 0, 960, 322);
    reelAnimation.setAnimationResetThreshold(0.0f);
    addAndMakeVisible(reelAnimation);

    vuMeterL.setNumFrames(65);
    vuMeterL.setDimensions(251, 518, 108, 108);
    vuMeterL.setAnimationImage(BinaryData::VUMeterL_png, BinaryData::VUMeterL_pngSize);
    addAndMakeVisible(vuMeterL);

    vuMeterR.setNumFrames(65);
    vuMeterR.setDimensions(605, 518, 110, 108);
    vuMeterR.setAnimationImage(BinaryData::VUMeterR_png, BinaryData::VUMeterR_pngSize);
    addAndMakeVisible(vuMeterR);

    ////////// LABELS //////////

    /*
    #if DEBUG
    juce::String debugText = "Debug Info...";
    debugLabel.setText(debugText, juce::dontSendNotification);
    debugLabel.setTopLeftPosition(100, 100);
    debugLabel.setFont(juce::Font (25.0f));
    debugLabel.setSize(500, 50);
    debugLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(debugLabel);
    #endif
    */

    int mainWidth = faceImage.getWidth();
    int mainHeight = faceImage.getHeight();
    setSize(mainWidth, mainHeight);

    showReelsAttachment.sendInitialUpdate();

    startTimer(25);
}

KissOfShameAudioProcessorEditor::~KissOfShameAudioProcessorEditor()
{
    inputSaturationKnob.removeListener(this);
    shameKnob.removeListener(this);
    outputKnob.removeListener(this);
    bypassButton.removeListener(this);
    linkIOButtonL.removeListener(this);
    linkIOButtonR.removeListener(this);
}

void KissOfShameAudioProcessorEditor::setShowReelsValue(float newValue)
{
    bool newVisible = newValue > 0.0f;
    bool oldVisible = getHeight() > 300;

    if (newVisible != oldVisible) {
        if (newVisible) {
            addAndMakeVisible(reelAnimation);
            setReelMode(true);
        } else {
            removeChildComponent(&reelAnimation);
            setReelMode(false);
        }
        setSize(faceImage.getWidth(), faceImage.getHeight());
        repaint();
    }
}

void KissOfShameAudioProcessorEditor::setReelMode(bool showReels)
{
    int adjustment = showReels ? 437 : -437;  // height difference: 703 - 266

    // Images
    backlight.setTopLeftPosition(backlight.getX(), backlight.getY() + adjustment);

    // Knobs
    inputSaturationKnob.setTopLeftPosition(inputSaturationKnob.getX(), inputSaturationKnob.getY() + adjustment);
    shameKnob.setTopLeftPosition(shameKnob.getX(), shameKnob.getY() + adjustment);
    hissKnob.setTopLeftPosition(hissKnob.getX(), hissKnob.getY() + adjustment);
    blendKnob.setTopLeftPosition(blendKnob.getX(), blendKnob.getY() + adjustment);
    outputKnob.setTopLeftPosition(outputKnob.getX(), outputKnob.getY() + adjustment);
    ageKnob.setTopLeftPosition(ageKnob.getX(), ageKnob.getY() + adjustment);

    // Buttons
    bypassButton.setTopLeftPosition(bypassButton.getX(), bypassButton.getY() + adjustment);
    tapeTypeButton.setTopLeftPosition(tapeTypeButton.getX(), tapeTypeButton.getY() + adjustment);
    printThroughButton.setTopLeftPosition(printThroughButton.getX(), printThroughButton.getY() + adjustment);

    // Components
    environmentsComponent.setTopLeftPosition(environmentsComponent.getX(), environmentsComponent.getY() + adjustment);

    // Animation
    vuMeterL.setTopLeftPosition(vuMeterL.getX(), vuMeterL.getY() + adjustment);
    vuMeterR.setTopLeftPosition(vuMeterR.getX(), vuMeterR.getY() + adjustment);
    shameKnobImage.setTopLeftPosition(shameKnobImage.getX(), shameKnobImage.getY() + adjustment);

    if (showReels) {
        faceImage.setAnimationImage(BinaryData::FaceWithReels_png, BinaryData::FaceWithReels_pngSize);
        faceImage.setDimensions(0, 0, 960, 703);
    } else {
        faceImage.setAnimationImage(BinaryData::Face_png, BinaryData::Face_pngSize);
        faceImage.setDimensions(0, 0, 960, 266);
    }
}

void KissOfShameAudioProcessorEditor::timerCallback()
{
    //debugLabel.setText(String(reelAnimation->getCurrentFlangeDepth()), juce::dontSendNotification);
    //debugLabel.setText(String(audioProcessor.curPositionInfo.isPlaying) + ":  " + String(audioProcessor.playHeadPos), juce::dontSendNotification);

    // Animation of VU meters
    //float smoothRMS = tanh(audioProcessor.curRMS*10);
    bool bypassed = audioProcessor.params.bypassParam->get();
    float vuLevelL = bypassed ? 0.0f : audioProcessor.curRMSL * 3.0f;
    float vuLevelR = bypassed ? 0.0f : audioProcessor.curRMSR * 3.0f;
    vuMeterL.updateImageWithValue(vuLevelL);
    vuMeterR.updateImageWithValue(vuLevelR);

    /*
    // Animation of backlighting
    if (!bypassed) {
        float backlightAlpha = 1.0f - (0.5f*audioProcessor.curRMSL + 0.5f*audioProcessor.curRMSR)*3.0f*float(shameKnob.getValue());
        backlight.setAlpha(backlightAlpha);
        shameKnob.setAlpha(backlightAlpha);
    }
    */

    // TODO: clean this up
    // TODO: don't animate when reels are not visible?
    //NOTE: when output level == 0, for some reason the AudioPlayhead position doesn't return to 0
    //after stopping playback. Don't know why this is... For now, only animating reels when output != 0.
    if (audioProcessor.curPositionInfo.isPlaying && audioProcessor.playHeadPos != priorProcessorTime && !bypassed) {
        priorProcessorTime = audioProcessor.playHeadPos;
        if (!reelAnimation.isAnimating) {
            reelAnimation.setFramesPerSecond(50);
            reelAnimation.isAnimating = true;
        }
    } else {
        reelAnimation.setFramesPerSecond(0);
        reelAnimation.isAnimating = false;
    }
}

void KissOfShameAudioProcessorEditor::mouseDoubleClick(const juce::MouseEvent&)
{
    if (audioProcessor.params.showReelsParam->get()) {
        showReelsAttachment.setValueAsCompleteGesture(0.0f);
    } else {
        showReelsAttachment.setValueAsCompleteGesture(1.0f);
    }
}

void KissOfShameAudioProcessorEditor::mouseDown(const juce::MouseEvent&)
{
    // do nothing
}

void KissOfShameAudioProcessorEditor::mouseUp(const juce::MouseEvent&)
{
    // do nothing
}

void KissOfShameAudioProcessorEditor::mouseDrag(const juce::MouseEvent&)
{
    // do nothing
}

void KissOfShameAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &inputSaturationKnob) {
        // Note: Changing the level parameter when Link is enabled only works
        // when the editor is open. It won't happen when the host changes the
        // link parameter directly.
        if (audioProcessor.params.linkParam->get()) {
            outputKnob.removeListener(this);
            float outputLevel = 1.0f - audioProcessor.params.inputParam->get();
            audioProcessor.params.outputParam->beginChangeGesture();
            audioProcessor.params.outputParam->setValueNotifyingHost(outputLevel);
            audioProcessor.params.outputParam->endChangeGesture();
            outputKnob.addListener(this);
        }
    }
    else if (slider == &outputKnob) {
        if (audioProcessor.params.linkParam->get()) {
            inputSaturationKnob.removeListener(this);
            float inputDrive = 1.0f - audioProcessor.params.outputParam->get();
            audioProcessor.params.inputParam->beginChangeGesture();
            audioProcessor.params.inputParam->setValueNotifyingHost(inputDrive);
            audioProcessor.params.inputParam->endChangeGesture();
            inputSaturationKnob.addListener(this);
        }
    }
    else if (slider == &shameKnob) {
        shameKnobImage.updateImageWithValue(float(slider->getValue()));
    }

    // The original code has this commented out for the Age knob,
    // which would also change the reel animation based on the age.
    //reelAnimation->setFramesPerSecond(ageKnob->getValue()*15 + 35);
    //reelAnimation->setAnimationRate(ageKnob->getValue());
    //reelAnimation->setAnimationResetThreshold(ageKnob->getValue()*0.025);
}

void KissOfShameAudioProcessorEditor::setBypassButtonValue(float newValue)
{
    const juce::ScopedValueSetter<bool> svs(ignoreCallbacks, true);
    bypassButton.setToggleState(newValue >= 0.5f, juce::sendNotificationSync);

    if (bypassButton.getToggleState()) {
        backlight.setAlpha(0.25f);
        shameKnob.setAlpha(0.25f);
    } else {
        backlight.setAlpha(1.0f);
        shameKnob.setAlpha(1.0f);
    }
}

void KissOfShameAudioProcessorEditor::setLinkButtonValue(float newValue)
{
    const juce::ScopedValueSetter<bool> svs(ignoreCallbacks, true);
    linkIOButtonL.setToggleState(newValue >= 0.5f, juce::sendNotificationSync);
    linkIOButtonR.setToggleState(newValue >= 0.5f, juce::sendNotificationSync);

    if (linkIOButtonL.getToggleState()) {
        linkIOButtonL.setAlpha(1.0f);
        linkIOButtonR.setAlpha(1.0f);
    } else {
        linkIOButtonL.setAlpha(0.25f);
        linkIOButtonR.setAlpha(0.25f);
    }
}

void KissOfShameAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (ignoreCallbacks) { return; }

    if (button == &bypassButton) {
        bypassButtonAttachment.setValueAsCompleteGesture(button->getToggleState() ? 1.0f : 0.0f);
    }
    else if (button == &linkIOButtonL) {
        linkButtonAttachment.setValueAsCompleteGesture(button->getToggleState() ? 1.0f : 0.0f);

        // TODO: Not sure if changing the input drive makes sense, since input
        // is used to apply saturation. The original plug-in didn't do this;
        // it always changed the output level to match the input level.
        if (button->getToggleState()) {
            float inputDrive = 1.0f - audioProcessor.params.outputParam->get();
            audioProcessor.params.inputParam->beginChangeGesture();
            audioProcessor.params.inputParam->setValueNotifyingHost(inputDrive);
            audioProcessor.params.inputParam->endChangeGesture();
        }
    }
    else if (button == &linkIOButtonR) {
        linkButtonAttachment.setValueAsCompleteGesture(button->getToggleState() ? 1.0f : 0.0f);

        // Note: Changing the level parameter when Link is enabled only works
        // when the editor is open. It won't happen when the host changes the
        // link parameter directly.
        if (button->getToggleState()) {
            float outputLevel = 1.0f - audioProcessor.params.inputParam->get();
            audioProcessor.params.outputParam->beginChangeGesture();
            audioProcessor.params.outputParam->setValueNotifyingHost(outputLevel);
            audioProcessor.params.outputParam->endChangeGesture();
        }
    }
}

void KissOfShameAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void KissOfShameAudioProcessorEditor::resized()
{
    // do nothing
}
