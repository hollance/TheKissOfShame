

#ifndef KOS_CustomKnob_h
#define KOS_CustomKnob_h

#include "../JuceLibraryCode/JuceHeader.h"



class CustomKnob : public Slider
{
public:
    CustomKnob();
    ~CustomKnob();
    
    void paint (Graphics& g);
    
private:

	Image knobImage;
    int knobFrameWidth;
    int knobFrameHeight;
    int knobNumFrames;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomKnob)
};

#endif
