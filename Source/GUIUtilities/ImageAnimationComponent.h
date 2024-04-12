#pragma once

#include "../shameConfig.h"

// Copied from juce::AnimatedAppComponent to allow framesPerSecond == 0
// since the original code depended on that but JUCE 7 doesn't allow it.
// TODO: use a less hacky solution (also support vblank)
class MyAnimatedAppComponent : public juce::Component, private juce::Timer
{
public:
    MyAnimatedAppComponent()
    {
        setOpaque(true);
    }

    void setFramesPerSecond(int framesPerSecond)
    {
        jassert(framesPerSecond >= 0 && framesPerSecond < 1000);
        if (framesPerSecond == 0) {
            stopTimer();
        } else {
            startTimerHz(framesPerSecond);
        }
    }

    virtual void update() = 0;

    int getFrameCounter() const noexcept { return totalUpdates; }

    int getMillisecondsSinceLastUpdate() const noexcept
    {
        return (int) (juce::Time::getCurrentTime() - lastUpdateTime).inMilliseconds();
    }

private:
    juce::Time lastUpdateTime = juce::Time::getCurrentTime();
    int totalUpdates = 0;

    void timerCallback() override
    {
        ++totalUpdates;
        update();
        repaint();
        lastUpdateTime = juce::Time::getCurrentTime();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyAnimatedAppComponent)
};

class ImageAnimationComponent : public MyAnimatedAppComponent, public juce::ActionBroadcaster
{
public:
    ImageAnimationComponent(juce::AudioParameterFloat& param, int numFrames,
                            const char* imageData, int imageSize)
        : parameter(param)
    {
        isAnimating = false;
        resetThresh = 0.0f;
        curIncrement = 0.0f;
        incrRate = 0.01f;

        animationNumFrames = numFrames;
        startFrame = 0;
        currentFrame = startFrame;
        endFrame = animationNumFrames - 1;

        animationImage = juce::ImageCache::getFromMemory(imageData, imageSize);
        imageFrameWidth = animationImage.getWidth();
        imageFrameHeight = animationImage.getHeight()/animationNumFrames;
        setSize(imageFrameWidth, imageFrameHeight);

        setFlangeDepth = 0.0f;

        setFramesPerSecond(0);  // initially stopped

        oglContext.setComponentPaintingEnabled(true);
        oglContext.attachTo(*this);
    }

    void mouseDown(const juce::MouseEvent&) override
    {
        setAnimationResetThreshold(0.015f);
        parameter.beginChangeGesture();
    }

    void mouseUp(const juce::MouseEvent&) override
    {
        parameter.endChangeGesture();
        setFlangeDepth = parameter.get();
        setAnimationResetThreshold(0.0f);
    }

    void mouseDrag(const juce::MouseEvent& event) override
    {
        float dragDist = float(event.getDistanceFromDragStartY()) / 100.0f;
        float curFlangeDepth = setFlangeDepth + dragDist;

        if (curFlangeDepth > 1.0f) { curFlangeDepth = 1.0f; }
        if (curFlangeDepth < 0.0f) { curFlangeDepth = 0.0f; }

        parameter.setValueNotifyingHost(curFlangeDepth);
    }

    void update() override
    {
        // Time to draw the next frame?
        if (curIncrement == 0.0f) {
            if (!animationImage.isNull()) {
                currentFrame++;
                if (currentFrame > endFrame) {
                    currentFrame = startFrame;
                }
            }
        }

        // Count until it's time to start the new frame.
        // Usually resetThresh is 0, which means we advance the animation
        // on every call to update(). When the mouse is down (flanging mode),
        // resetThresh is greater than 0 and the animation slows down.
        curIncrement += incrRate;
        if (curIncrement >= resetThresh) {
            curIncrement = 0.0f;
        }
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black);

        if (!animationImage.isNull()) {
            // Grab the current animation frame from the filmstrip.
            juce::Rectangle<int> clipRect(0, currentFrame * imageFrameHeight,
                                          imageFrameWidth, imageFrameHeight);
            const juce::Image& clippedIm = animationImage.getClippedImage(clipRect);

            // NOTE: for some reason, had to offset drawing the image by
            // 1 pixel to avoid grey line showing break between images.
            // Something strange about the openGL context...
            g.drawImageAt(clippedIm, 0, 1);
        }
    }

    void setFrameDimensions(int topLeftX, int topLeftY, int w, int h)
    {
        setTopLeftPosition(topLeftX, topLeftY);
        imageFrameWidth = w;
        imageFrameHeight = h;
    }

    void setStartingFrame(int frameNumber)
    {
        startFrame = frameNumber;
        if (currentFrame < startFrame) {
            currentFrame = startFrame;
        }
    }

    void setEndingFrame(int frameNumber)
    {
        endFrame = frameNumber;
    }

    void setAnimationRate(float rate)
    {
        incrRate = rate;
    }

    void setAnimationResetThreshold(float thresh)
    {
        resetThresh = thresh;
    }

    bool isAnimating;

private:
    juce::AudioParameterFloat& parameter;
    juce::OpenGLContext oglContext;

    float resetThresh;
    float curIncrement;
    float incrRate;

    juce::Image animationImage;
    int imageFrameWidth;
    int imageFrameHeight;

    int animationNumFrames;
    int currentFrame;
    int startFrame;
    int endFrame;

    float setFlangeDepth;
};
