

#ifndef KOS_ImageInteractor_h
#define KOS_ImageInteractor_h

#include "../JuceLibraryCode/JuceHeader.h"



class ImageInteractor : public Component, public ChangeListener
{
public:
    ImageInteractor();
    ~ImageInteractor();
    
    virtual void changeListenerCallback (ChangeBroadcaster* source)
    {
        
    };
    
    void paint (Graphics& g);
    
    
    void updateImageWithValue(float value)
    {
        if(value > maxValue)
        {
            curValue = maxValue;
        }
        else if(value < minValue)
        {
            curValue = minValue;
        }
        else
        {
            curValue = value;
        }
        
        repaint();
    }
    
    void setCurrentValue(float value){curValue = value;}
    
    void setMinMaxValues(float min, float max)
    {
        minValue = min;
        maxValue = max;
    }
    
    
    
private:

	Image knobImage;
    int knobFrameWidth;
    int knobFrameHeight;
    int knobNumFrames;
    
    float maxValue, minValue, curValue;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageInteractor)
};

#endif
