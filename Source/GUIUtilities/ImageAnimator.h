//
//  ImageAnimator.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/4/14.
//
//

#ifndef __KissOfShame__ImageAnimator__
#define __KissOfShame__ImageAnimator__


#include "../JuceLibraryCode/JuceHeader.h"

class ImageAnimator : public Component, Timer
{
  
public:
    ImageAnimator(File imgFile, int numFrames, int frameRateInMilliseconds) : animationNumFrames(numFrames), frameRate(frameRateInMilliseconds), isAnimating(false)
    {
        startFrame = 0;
        currentFrame = startFrame;
        curFramePosition = startFrame;
        endFrame = animationNumFrames - 1;
        animationRate = 0.8;
        
        animationImage = ImageCache::getFromFile(imgFile);
        imageFrameWidth = animationImage.getWidth();
        imageFrameHeight = animationImage.getHeight()/animationNumFrames;
        setSize(imageFrameWidth, imageFrameHeight);
    }
    
    ~ImageAnimator(){}
    
    virtual void mouseDown (const MouseEvent& event){};
    virtual void mouseUp (const MouseEvent& event){};
    virtual void mouseDrag (const MouseEvent& event)
    {
        float dragDist = -(float)event.getDistanceFromDragStartY()/3000;
        
        animationRate += dragDist;
        
        if(animationRate > 1.25) animationRate = 1.25;
        if(animationRate < 0.75) animationRate = 0.75;
        
        
//        event.getMouseDownScreenY();
//        event.getPosition().getY();
    }
    
    float getAnimationRate() {return animationRate; }

    
    void paint (Graphics& g)
    {
        if (!animationImage.isNull()/* && isAnimating*/)
        {
            if(currentFrame > endFrame) currentFrame = startFrame;
            //if(curFramePosition > endFrame) curFramePosition -= animationNumFrames;
            
            //currentFrame = (int)curFramePosition;
            
            juce::Rectangle<int> clipRect(0, currentFrame*imageFrameHeight, imageFrameWidth, imageFrameHeight);
            const Image & clippedIm = animationImage.getClippedImage(clipRect);
            g.drawImageAt(clippedIm, 0, 0);
            
            //curFramePosition += animationRate;
            currentFrame++;// = (int)curFramePosition;
            //currentFrame += 5; can change the speed via the increment amount, but jitter will result...
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
    
    void setFrameRate(int milliseconds)
    {
        frameRate = milliseconds;
    }
    
    void startAnimation()
    {
        isAnimating = true;
        startTimer(frameRate);
    }
    
    void stopAnimation()
    {
        isAnimating = false;
        stopTimer();
    }
    
    virtual void timerCallback()
    {
        repaint();
    }
    
    bool isAnimating;
    
private:
    
	Image animationImage;
    int imageFrameWidth;
    int imageFrameHeight;
    
    
    int animationNumFrames;
    int currentFrame;
    int startFrame;
    int endFrame;
    int frameRate;
    float animationRate;
    float curFramePosition;
    
    
};



#endif /* defined(__KissOfShame__ImageAnimator__) */
