#include "ImageInteractor.h"


ImageInteractor::ImageInteractor()
:
numFrames(128), curValue(0)
{
    minValue = 0.0;
    maxValue = 1.0;
    
    imagePath = GUI_PATH + "MixKnob/Knob-Pan-Mix.png";
    
    File imgFile;
    imgFile = File(imagePath);
    image = ImageCache::getFromFile(imgFile);
    frameWidth = image.getWidth();
    frameHeight = image.getHeight()/numFrames;
    setSize(frameWidth, frameHeight);
    
    
    setInterceptsMouseClicks(false, false);
}

ImageInteractor::~ImageInteractor()
{}


void ImageInteractor::setNumFrames(int _numFrames)
{
    curValue = 0;
    numFrames = _numFrames;
}

void ImageInteractor::setAnimationImage(String filePath)
{
    image = ImageCache::getFromFile(File(filePath));
}

void ImageInteractor::setDimensions(int topLeftX, int topLeftY, int w, int h)
{
    setTopLeftPosition(topLeftX, topLeftY);
    frameWidth = w;
    frameHeight = h;
    setSize(frameWidth, frameHeight);
}


void ImageInteractor::paint (Graphics& g)
{
    if (!image.isNull())
    {
        double normalizedValue = (curValue - minValue) / (maxValue - minValue);
        int frameNum = normalizedValue*(numFrames-1);
		juce::Rectangle<int> clipRect(0, frameNum*frameHeight, frameWidth, frameHeight);
        const Image & clippedIm = image.getClippedImage(clipRect);
        g.drawImageAt(clippedIm, 0, 0);
    }
}


