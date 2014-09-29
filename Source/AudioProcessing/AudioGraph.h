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
#include "Blend.h"


class AudioGraph
{
public:
    
    
    AudioGraph(int numChannels)
    {
        inSaturation = new InputSaturation(0.1, 2.0);
        
        shame = new Shame(2);
        shame->setDepth(0.2);
        shame->setRate(7);
        
        hiss = new Hiss();
        
        blend = new Blend();
        
        bypassGraph = false;
        outputLevel = 1.0;
    }
    
    ~AudioGraph(){}
    
        
    void processGraph(AudioSampleBuffer& audioBuffer, int numChannels)
    {
        if(bypassGraph) return;
        
        //make a copy of the original audio to be used strictly for processing
        audioGraphProcessingBuffer = audioBuffer;
        
        //process audio
        inSaturation->processInputSaturation(audioGraphProcessingBuffer, numChannels);
        hiss->processHiss(audioGraphProcessingBuffer, numChannels);
        shame->processShame(audioGraphProcessingBuffer, numChannels);
        blend->processBlend(audioBuffer, audioGraphProcessingBuffer, numChannels);
        
        //apply the final output level
        audioBuffer.applyGain(outputLevel);        
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
            
            case eShameFreq: shame->setRate(paramLevel); break;
            case eShameDepth: shame->setDepth(paramLevel); break;
            case eShameGlobalLevel: shame->setRate(paramLevel); break;
                
            case eHissLevel:   hiss->setHissLevel(paramLevel); break;
                
            case eBlendLevel:  blend->setBlendLevel(paramLevel); break;
                
            case eBypass:      bypassGraph = paramLevel; break;
                
            case eOutputLevel: outputLevel = paramLevel; break;
                
            default: break;
        }
    }

    
private:
    
    AudioSampleBuffer audioGraphProcessingBuffer;
    
    ScopedPointer<Hiss> hiss;
    ScopedPointer<Shame> shame;
    ScopedPointer<InputSaturation> inSaturation;
    ScopedPointer<Blend> blend;
    
    bool bypassGraph;
    float outputLevel;
    
};


#endif /* defined(__KissOfShame__AudioGraph__) */
