//
//  AudioGraph.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/9/14.
//
//

#ifndef __KissOfShame__AudioGraph__
#define __KissOfShame__AudioGraph__

#include <iostream>

#include "../shameConfig.h"
#include "Hiss.h"
#include "Shame.h"
#include "InputSaturation.h"


class AudioGraph
{
public:
    
    
    AudioGraph(int numChannels)
    {
        inSaturation = new InputSaturation(0.1, 2.0);
        
        shame = new Shame(2);
        shame->setOscDepth(0.1);
        shame->setOscFreq(7);
        
        hiss = new Hiss();
    }
    
    ~AudioGraph(){}
    
        
    void processGraph(AudioSampleBuffer& audioBuffer, int numChannels)
    {
        inSaturation->processInputSaturation(audioBuffer, numChannels);
        shame->processShame(audioBuffer, numChannels);
        hiss->processHiss(audioBuffer, numChannels);
    }
    
    
    void setAudioUnitParameters(AUParameter param, float paramLevel)
    {
        switch(param)
        {
            case eSaturationDrive: inSaturation->setDrive(paramLevel); break;
            case eSaturationOuput: inSaturation->setOutput(paramLevel); break;
            case eSaturationRate: inSaturation->setRate(paramLevel); break;
            case eSaturationThresh: inSaturation->setThreshold(paramLevel); break;
            case eSaturationGlobalLevel: inSaturation->setGlobalLevel(paramLevel); break;
            
            case eShameFreq: shame->setOscFreq(paramLevel); break;
            case eShameDepth: shame->setOscDepth(paramLevel); break;
            case eShameGlobalLevel: shame->setOscFreq(paramLevel); break;
                
            case eHissLevel: hiss->setHissLevel(paramLevel); break;
                
            default: break;
        }
    }

    
private:
    
    ScopedPointer<Hiss> hiss;
    ScopedPointer<Shame> shame;
    ScopedPointer<InputSaturation> inSaturation;
    
};


#endif /* defined(__KissOfShame__AudioGraph__) */
