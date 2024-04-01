

#ifndef KOS_CustomKnob_h
#define KOS_CustomKnob_h

#include "../JuceLibraryCode/JuceHeader.h"

using namespace juce;


class CustomKnob : public Slider
{
public:
    CustomKnob();
    ~CustomKnob();
    
    void setNumFrames(int numFrames);
    void setKnobImage(String filePath);
    void setKnobDimensions(int topLeftX, int topLeftY, int w, int h);
    
    void paint (Graphics& g);
        
private:
    
    String knobImagePath;
    int knobNumFrames;
    
	Image knobImage;
    int knobFrameWidth;
    int knobFrameHeight;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomKnob)
};

#endif
