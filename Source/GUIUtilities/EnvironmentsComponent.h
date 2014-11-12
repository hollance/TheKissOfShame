

#ifndef KOS_EnvironmentsComponent_h
#define KOS_EnvironmentsComponent_h

#include "ImageInteractor.h"



class EnvironmentsComponent : public ImageInteractor, public MouseListener
{
public:
    
    EnvironmentsComponent() : imageIncr(0)
    {
        setNumFrames(6);
        setMinMaxValues(0, 5);
        
        String environmentsImageLocation = GUI_PATH + "KOS_Graphics/00.png";
        setAnimationImage(environmentsImageLocation);
        
        setDimensions(0, 0, 183, 32);
        setSize(183, 32);
    };
    
    ~EnvironmentsComponent(){};
    
    
    virtual void mouseUp (const MouseEvent& event)
    {
        imageIncr = (imageIncr + 1) % 6;
        updateImageWithValue(imageIncr);
    }
    
    virtual void mouseDrag(const MouseEvent& event){}
    
private:
    
    int imageIncr;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvironmentsComponent)
};


#endif
