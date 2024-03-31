//
//  ImageAnimator.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/4/14.
//
//

#ifndef __KissOfShame__ImageAnimationComponent__
#define __KissOfShame__ImageAnimationComponent__


#include "../shameConfig.h"


// Copied from juce::AnimatedAppComponent to allow framesPerSecond == 0
// TODO: use a less hacky solution (also support vblank)
class MyAnimatedAppComponent : public Component, private Timer
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
        return (int) (Time::getCurrentTime() - lastUpdateTime).inMilliseconds();
    }

private:
    Time lastUpdateTime = Time::getCurrentTime();
    int totalUpdates = 0;

    void timerCallback() override
    {
        ++totalUpdates;
        update();
        repaint();
        lastUpdateTime = Time::getCurrentTime();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyAnimatedAppComponent)
};

class ImageAnimationComponent : public MyAnimatedAppComponent, public ActionBroadcaster
{
  
public:
    ImageAnimationComponent(File imgFile, int numFrames, int framesPerSecond) : animationNumFrames(numFrames), isAnimating(false), curIncrement(0), resetThresh(1), incrRate(0.01)
    {
        setFramesPerSecond(framesPerSecond); //NOTE: setting framesPerSecond to 0 stops the timer.
        
        startFrame = 0;
        currentFrame = startFrame;
        curFramePosition = startFrame;
        endFrame = animationNumFrames - 1;
        
        animationImage = ImageCache::getFromFile(imgFile);
        imageFrameWidth = animationImage.getWidth();
        imageFrameHeight = animationImage.getHeight()/animationNumFrames;
        setSize(imageFrameWidth, imageFrameHeight);
        
        oglContext.setComponentPaintingEnabled(true);
        oglContext.attachTo(*this);
    }
    
    virtual void mouseDown (const MouseEvent& event)
    {
        //setFramesPerSecond(25);
        setAnimationResetThreshold(0.015);
    };
    virtual void mouseUp (const MouseEvent& event)
    {
        //setFramesPerSecond(50);
        setFlangeDepth = curFlangeDepth;
        setAnimationResetThreshold(0.0);
    };
    virtual void mouseDrag (const MouseEvent& event)
    {
        dragDist = (float)event.getDistanceFromDragStartY()/100;
        curFlangeDepth = setFlangeDepth + dragDist;
        
        if(curFlangeDepth > 1.0) curFlangeDepth = 1.0;
        if(curFlangeDepth < 0) curFlangeDepth = 0;
        sendActionMessage("updateFlange");
    }
    
    float getCurrentFlangeDepth() {return curFlangeDepth;}

    
    ~ImageAnimationComponent(){}
    
    void update() override
    {
    // This function is called at the frequency specified by the setFramesPerSecond() call
    // in the constructor. You can use it to update counters, animate values, etc.
        
        if(curIncrement == 0)
        {
            if (!animationImage.isNull()/* && isAnimating*/)
            {
                if(currentFrame >= endFrame) currentFrame = startFrame;
                currentFrame++;// = (int)curFramePosition;
            }
        }
    
        curIncrement += incrRate;
        if(curIncrement >= resetThresh) curIncrement = 0;
            
    }

    void paint (Graphics& g)
    {
        g.fillAll(Colours::black);
        
        if (!animationImage.isNull()/* && isAnimating*/)
        {
            juce::Rectangle<int> clipRect(0, currentFrame*imageFrameHeight, imageFrameWidth, imageFrameHeight);
            const Image & clippedIm = animationImage.getClippedImage(clipRect);
            
            //NOTE: for some reason, had to offset drawing the image by 1 pixel to avoid grey line showing break between images. Something strange about the openGL context...
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
        if(currentFrame < startFrame) currentFrame = startFrame;
    }
    
    void setEndingFrame(int frameNumber)
    {
        endFrame = frameNumber;
    }

    void setAnimationRate(float rate) {incrRate = rate;}
    void setAnimationResetThreshold(float thresh) {resetThresh = thresh;}


    bool isAnimating;

private:

    OpenGLContext oglContext;

    float resetThresh;
    float curIncrement;
    float incrRate;

	Image animationImage;
    int imageFrameWidth;
    int imageFrameHeight;

    float curFlangeDepth;
    float setFlangeDepth;
    float dragDist;

    int animationNumFrames;
    int currentFrame;
    int startFrame;
    int endFrame;
    float curFramePosition;
};



#endif /* defined(__KissOfShame__ImageAnimator__) */
