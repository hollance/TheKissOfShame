#pragma once

#include "../shameConfig.h"

class CustomButton : public juce::ImageButton
{
public:
    CustomButton() { }

    void resizeButton(float scale)
    {
        offImage = offImage.rescaled(int(offImage.getWidth() * scale),
                                     int(offImage.getHeight() * scale));
        onImage = onImage.rescaled(int(onImage.getWidth() * scale),
                                   int(onImage.getHeight() * scale));
        updateImages();
    }

    void setClippedCustomOnImage(const char* imageData, int imageSize, int topLeftX, int topLeftY, int w, int h)
    {
        onImage = juce::ImageCache::getFromMemory(imageData, imageSize);

        if (!onImage.isNull()) {
            juce::Rectangle<int> clipRect(topLeftX, topLeftY, w, h);
            onImage = onImage.getClippedImage(clipRect);
        }

        updateImages();

        setSize(onImage.getWidth(), onImage.getHeight());
    }

    void setClippedCustomOffImage(const char* imageData, int imageSize, int topLeftX, int topLeftY, int w, int h)
    {
        offImage = juce::ImageCache::getFromMemory(imageData, imageSize);

        if (!offImage.isNull()) {
            juce::Rectangle<int> clipRect(topLeftX, topLeftY, w, h);
            offImage = offImage.getClippedImage(clipRect);
        }

        updateImages();
    }

private:
    void updateImages()
    {
        setImages(true, false, true,
                  onImage, 1.0f, juce::Colour(0x0),
                  juce::Image(), 1.0f, juce::Colour(0x0),
                  offImage, 1.0f, juce::Colour(0x0));
    }

    juce::Image offImage;
    juce::Image onImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomButton)
};

