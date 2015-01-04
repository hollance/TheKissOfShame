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


class ImageAnimationComponent : public AnimatedAppComponent, public ActionBroadcaster
{
  
public:
    ImageAnimationComponent(File imgFile, int numFrames, int framesPerSecond) : animationNumFrames(numFrames), isAnimating(false)
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
    }
    
    virtual void mouseDown (const MouseEvent& event)
    {
        setFramesPerSecond(25);
    };
    virtual void mouseUp (const MouseEvent& event)
    {
        setFramesPerSecond(50);
        setFlangeDepth = curFlangeDepth;
    };
    virtual void mouseDrag (const MouseEvent& event)
    {
        dragDist = (float)event.getDistanceFromDragStartY()/100;
        curFlangeDepth = setFlangeDepth + dragDist;
        
        if(curFlangeDepth > 1.0) curFlangeDepth = 1.0;
        if(curFlangeDepth < 0) curFlangeDepth = 0;
        sendActionMessage("updateFlange");
        
        //setFramesPerSecond(50 - 10*curFlangeDepth);
    }
    
    float getCurrentFlangeDepth() {return curFlangeDepth;}

    
    ~ImageAnimationComponent(){}
    
    void update() override
    {
    // This function is called at the frequency specified by the setFramesPerSecond() call
    // in the constructor. You can use it to update counters, animate values, etc.
        if (!animationImage.isNull()/* && isAnimating*/)
        {
            if(currentFrame >= endFrame) currentFrame = startFrame;
                currentFrame++;// = (int)curFramePosition;
        }
    }


    void paint (Graphics& g)
    {
        if (!animationImage.isNull()/* && isAnimating*/)
        {
            juce::Rectangle<int> clipRect(0, currentFrame*imageFrameHeight, imageFrameWidth, imageFrameHeight);
            const Image & clippedIm = animationImage.getClippedImage(clipRect);
            g.drawImageAt(clippedIm, 0, 0);
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
    

    bool isAnimating;

private:
    
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
