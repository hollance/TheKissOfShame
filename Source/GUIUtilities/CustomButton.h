#pragma once

#include "../shameConfig.h"

class CustomButton : public juce::ImageButton
{
public:
    CustomButton()
    {
        // TODO: do we need these?
        juce::File offImgFile(juce::String(GUI_PATH) + "testButton-off.png");
        juce::File onImgFile(juce::String(GUI_PATH) + "testButton-on.png");

        offImage = juce::ImageCache::getFromFile(offImgFile);
        onImage = juce::ImageCache::getFromFile(onImgFile);

        updateImages();

        setSize(onImage.getWidth(), onImage.getHeight());
    }

    void resizeButton(float scale)
    {
        offImage = offImage.rescaled(int(offImage.getWidth() * scale),
                                     int(offImage.getHeight() * scale));
        onImage = onImage.rescaled(int(onImage.getWidth() * scale),
                                   int(onImage.getHeight() * scale));
        updateImages();
    }

    void setImagePaths(const juce::String& onImgPath, const juce::String& offImgPath)
    {
        juce::File offImgFile(juce::String(GUI_PATH) + onImgPath);
        juce::File onImgFile(juce::String(GUI_PATH) + offImgPath);

        offImage = juce::ImageCache::getFromFile(offImgFile);
        onImage = juce::ImageCache::getFromFile(onImgFile);

        updateImages();
    }

    void setClippedCustomOnImage(const juce::String& onImgPath, int topLeftX, int topLeftY, int w, int h)
    {
        onImage = juce::ImageCache::getFromFile(juce::File(onImgPath));

        if (!onImage.isNull()) {
            juce::Rectangle<int> clipRect(topLeftX, topLeftY, w, h);
            onImage = onImage.getClippedImage(clipRect);
        }

        updateImages();
    }

    void setClippedCustomOffImage(const juce::String& offImgPath, int topLeftX, int topLeftY, int w, int h)
    {
        offImage = juce::ImageCache::getFromFile(juce::File(offImgPath));

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

