#include "ImageInteractor.h"


const String knobImagePath = "/Users/brianhansen/Documents/Brian/Work/1_KOS/kissofshame/GUI_Resources/MixKnob/Knob-Pan-Mix.png";

ImageInteractor::ImageInteractor()
:
knobNumFrames(128)
{
    
    minValue = 0.0;
    maxValue = 1.0;
    
    File imgFile;
    imgFile = File(knobImagePath);
    knobImage = ImageCache::getFromFile(imgFile);
    knobFrameWidth = knobImage.getWidth();
    knobFrameHeight = knobImage.getHeight()/knobNumFrames;
    setSize(knobFrameWidth, knobFrameHeight);

}

ImageInteractor::~ImageInteractor()
{}

void ImageInteractor::paint (Graphics& g)
{
    if (!knobImage.isNull())
    {
        double normalizedValue = (curValue - minValue) / (maxValue - minValue);
        int frameNum = normalizedValue*(knobNumFrames-1);
		juce::Rectangle<int> clipRect(0, frameNum*knobFrameHeight, knobFrameWidth, knobFrameHeight);
        const Image & clippedIm = knobImage.getClippedImage(clipRect);
        g.drawImageAt(clippedIm, 0, 0);
    }
}


