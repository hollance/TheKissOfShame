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
    ImageAnimator(File imgFile, int numFrames, int frameRateInMilliseconds) : animationNumFrames(numFrames), frameRate(frameRateInMilliseconds)
    {
        startFrame = 0;
        currentFrame = startFrame;
        endFrame = animationNumFrames - 1;
        
        animationImage = ImageCache::getFromFile(imgFile);
        imageFrameWidth = animationImage.getWidth();
        imageFrameHeight = animationImage.getHeight()/animationNumFrames;
        setSize(imageFrameWidth, imageFrameHeight);
    }
    
    ~ImageAnimator(){} 
    
    void paint (Graphics& g)
    {
        if (!animationImage.isNull())
        {
            if(currentFrame > endFrame) currentFrame = startFrame;
            
            juce::Rectangle<int> clipRect(0, currentFrame*imageFrameHeight, imageFrameWidth, imageFrameHeight);
            const Image & clippedIm = animationImage.getClippedImage(clipRect);
            g.drawImageAt(clippedIm, 0, 0);
            
            currentFrame++;
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
        startTimer(frameRate);
    }
    
    void stopAnimation()
    {
        stopTimer();
    }
    
    virtual void timerCallback()
    {
        repaint();
    }
    
    
private:
    
	Image animationImage;
    int imageFrameWidth;
    int imageFrameHeight;
    
    
    int animationNumFrames;
    int currentFrame;
    int startFrame;
    int endFrame;
    int frameRate;
};



#endif /* defined(__KissOfShame__ImageAnimator__) */
