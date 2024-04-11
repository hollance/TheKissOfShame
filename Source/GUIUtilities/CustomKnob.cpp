#include "CustomKnob.h"

CustomKnob::CustomKnob() : knobNumFrames(128)
{
    setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    setRange(0.000f, 1.000f, 0.001f);
    setValue(0.0f);
}

void CustomKnob::setNumFrames(int numFrames)
{
    knobNumFrames = numFrames;
}

void CustomKnob::setKnobImage(const juce::String& filePath)
{
    knobImage = juce::ImageCache::getFromFile(juce::File(filePath));
}

void CustomKnob::setKnobDimensions(int topLeftX, int topLeftY, int w, int h)
{
    setTopLeftPosition(topLeftX, topLeftY);
    knobFrameWidth = w;
    knobFrameHeight = h;
    setSize(knobFrameWidth, knobFrameHeight);
}

void CustomKnob::paint(juce::Graphics& g)
{
    if (!knobImage.isNull()) {
        double normalizedValue = (getValue() - getMinimum()) / (getMaximum() - getMinimum());
        int frameNum = int(normalizedValue * (double(knobNumFrames - 1)));
		juce::Rectangle<int> clipRect(0, frameNum * knobFrameHeight, knobFrameWidth, knobFrameHeight);
        const juce::Image& clippedIm = knobImage.getClippedImage(clipRect);
        g.drawImageAt(clippedIm, 0, 0);
    }
}
