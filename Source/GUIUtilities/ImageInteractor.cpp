#include "ImageInteractor.h"

ImageInteractor::ImageInteractor(float startValue) : desaturate(false), numFrames(128), curValue(startValue)
{
    minValue = 0.0f;
    maxValue = 1.0f;
}

void ImageInteractor::setNumFrames(int _numFrames)
{
    curValue = 0.0f;
    numFrames = _numFrames;
}

void ImageInteractor::setAnimationImage(const char* data, int size)
{
    image = juce::ImageCache::getFromMemory(data, size);
    desatImage = image.createCopy();
    satImage = image.createCopy();
    desatImage.desaturate();
}

void ImageInteractor::setDimensions(int topLeftX, int topLeftY, int w, int h)
{
    setTopLeftPosition(topLeftX, topLeftY);
    frameWidth = w;
    frameHeight = h;
    setSize(frameWidth, frameHeight);
}

void ImageInteractor::paint(juce::Graphics& g)
{
    if (!image.isNull()) {
        double normalizedValue = (curValue - minValue) / (maxValue - minValue);
        int frameNum = int(normalizedValue * double(numFrames - 1));
        juce::Rectangle<int> clipRect(0, frameNum*frameHeight, frameWidth, frameHeight);
        image = desaturate ? desatImage : satImage;
        const juce::Image& clippedIm = image.getClippedImage(clipRect);
        g.drawImageAt(clippedIm, 0, 0);
    }
}
