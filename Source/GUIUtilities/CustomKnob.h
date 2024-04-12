#pragma once

#include <JuceHeader.h>

class CustomKnob : public juce::Slider
{
public:
    CustomKnob();

    void setNumFrames(int numFrames);
    void setKnobImage(const char* data, int size);
    void setKnobDimensions(int topLeftX, int topLeftY, int w, int h);

    void paint(juce::Graphics& g);

private:
    juce::String knobImagePath;
    int knobNumFrames;

    juce::Image knobImage;
    int knobFrameWidth;
    int knobFrameHeight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomKnob)
};
