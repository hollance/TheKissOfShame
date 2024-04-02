

#ifndef KOS_EnvironmentsComponent_h
#define KOS_EnvironmentsComponent_h

#include "ImageInteractor.h"
#include "../shameConfig.h"
#include "../PluginProcessor.h"

using namespace juce;


class EnvironmentsComponent : public ImageInteractor, public MouseListener
{
public:
    
    EnvironmentsComponent(KissOfShameAudioProcessor& p) : imageIncr(0), curEnvironment(eEnvironmentOff), processor(p)
    {
        setNumFrames(6);
        setMinMaxValues(0, 5);
        
        String environmentsImageLocation = GUI_PATH + "KOS_Graphics/00.png";
        setAnimationImage(environmentsImageLocation);
        
        setDimensions(0, 0, 183, 32);
        setSize(183, 32);
    }
    
    ~EnvironmentsComponent(){}
    
    
    
    void setCurrentEnvironment(int index)
    {
        switch (index)
        {
            case 0:
                curEnvironment = eEnvironmentOff;
                break;
            case 1:
                curEnvironment = eEnvironmentEnvironment;
                break;
            case 2:
                curEnvironment = eEnvironmentStudioCloset;
                break;
            case 3:
                curEnvironment = eEnvironmentHumidCellar;
                break;
            case 4:
                curEnvironment = eEnvironmentHotLocker;
                break;
            case 5:
                curEnvironment = eEnvironmentHurricaneSandy;
                break;
                
            default:
                break;
        }

        // TODO: not thread safe
        processor.audioGraph.setCurrentEnvironment(curEnvironment);
    }
        
    
    virtual void mouseUp ([[maybe_unused]] const MouseEvent& event)
    {
        imageIncr = (imageIncr + 1) % 6;
        updateImageWithValue(imageIncr);
        
        
        setCurrentEnvironment(imageIncr);
    }
    
    virtual void mouseDrag([[maybe_unused]] const MouseEvent& event) {}

private:
    
    int imageIncr;
    EShameEnvironments curEnvironment;
    
    KissOfShameAudioProcessor& processor;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvironmentsComponent)
};


#endif
