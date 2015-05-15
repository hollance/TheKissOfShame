

#ifndef KOS_BacklightComponent_h
#define KOS_BacklightComponent_h

#include "../shameConfig.h"



class BacklightComponent : public Component
{
public:
    
    BacklightComponent()
    {
        setSize(960, 266);
        setInterceptsMouseClicks(false, false);
    };
    
    ~BacklightComponent(){};
    
    
    void paint (Graphics& g)
    {
        g.fillAll(Colour::fromFloatRGBA(1.0f, 0.216f, 0.384f, 1.0));
    }
    
private:
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BacklightComponent)
};


#endif
