#include "CustomKnob.h"



CustomKnob::CustomKnob()
:
knobNumFrames(128)
{
    setSliderStyle(Rotary);
    setTextBoxStyle(NoTextBox, true, 0, 0);
    setRange(0.000, 1.000, 0.001);
    setValue(0.0);
    
    
    knobImagePath = "/Users/brianhansen/Documents/Brian/Work/1_KOS/kissofshame/GUI_Resources/MixKnob/Knob-Pan-Mix.png";
    
    File imgFile;
    imgFile = File(knobImagePath);
    knobImage = ImageCache::getFromFile(imgFile);
    knobFrameWidth = knobImage.getWidth();
    knobFrameHeight = knobImage.getHeight()/knobNumFrames;
    setSize(knobFrameWidth, knobFrameHeight);
    
    
}

CustomKnob::~CustomKnob()
{}

void CustomKnob::setNumFrames(int numFrames)
{
    knobNumFrames = numFrames;
    setValue(0.0);
}

void CustomKnob::setKnobImage(String filePath)
{
    knobImage = ImageCache::getFromFile(File(filePath));
}


void CustomKnob::setKnobDimensions(int topLeftX, int topLeftY, int w, int h)
{
    setTopLeftPosition(topLeftX, topLeftY);
    knobFrameWidth = w;
    knobFrameHeight = h;
    setSize(knobFrameWidth, knobFrameHeight);
}


void CustomKnob::paint (Graphics& g)
{
    if (!knobImage.isNull())
    {
        double normalizedValue = (getValue() - getMinimum()) / (getMaximum() - getMinimum());
        int frameNum = normalizedValue*(knobNumFrames-1);
		juce::Rectangle<int> clipRect(0, frameNum*knobFrameHeight, knobFrameWidth, knobFrameHeight);
        const Image & clippedIm = knobImage.getClippedImage(clipRect);
        g.drawImageAt(clippedIm, 0, 0);
    }
}


