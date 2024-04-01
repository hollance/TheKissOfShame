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
#include "Flange.h"
#include "HurricaneSandy.h"


class AudioGraph
{
public:
    
    
    AudioGraph(int /*numChannels*/) : currentEnvironment(eEnvironmentOff)
    {
        inSaturation.reset(new InputSaturation(0.0f, 2.0f, 0.272f));

        flange.reset(new Flange(2));
        flange->setDepth(0.0);
        
        shame.reset(new Shame(2));
        shame->setInterpolatedParameters(0.0);
        
        hiss.reset(new Hiss());

        blend.reset(new Blend());

        hurricaneSandy.reset(new HurricaneSandy());


        bypassGraph = false;
        
        outputLevel = 1.0;
        inputDrive = 1.0;
    }
    
    ~AudioGraph(){}
    
    
    void setCurrentEnvironment(EShameEnvironments env) {currentEnvironment = env;}
        
    void processGraph(AudioSampleBuffer& audioBuffer, int numChannels)
    {
        if(bypassGraph) return;
        
        //apply the input drive
        audioBuffer.applyGain(inputDrive);
        
        //make a copy of the original audio to be used strictly for processing
        audioGraphProcessingBuffer = audioBuffer;
        
        //////////// Process Audio ///////////
        //1.Incomming audio gets flange and saturation processing
        inSaturation->processInputSaturation(audioGraphProcessingBuffer, numChannels);
        flange->processFlange(audioGraphProcessingBuffer, numChannels);
        
        //2. add environment effects
        //EShameEnvironments environmentSelection = eEnvironmentHurricaneSandy;
        //switch (environmentSelection)
        switch (currentEnvironment)
        {
            case eEnvironmentHurricaneSandy:
                hurricaneSandy->processHurricaneSandy(audioGraphProcessingBuffer, numChannels);
                break;
            case eEnvironmentHotLocker:
                break;
            case eEnvironmentHumidCellar:
                break;
            case eEnvironmentStudioCloset:
                break;
            case eEnvironmentOff:
                break;
            case eEnvironmentEnvironment:
                break;
            case eEnvironmentTotalEnvironments:
                break;
            default:
                break;
        }
        
        //3. add hiss and the shame feature
        hiss->processHiss(audioGraphProcessingBuffer, numChannels);
        shame->processShame(audioGraphProcessingBuffer, numChannels);
        
        //4. blend the processed audio with the original signal.
        blend->processBlend(audioBuffer, audioGraphProcessingBuffer, numChannels);
        
        //////////// End Process Audio ///////////
        
        //5. apply the final output level
        audioBuffer.applyGain(outputLevel);
    }
    
    
    void setInputDrive(float drive)
    {
        //NOTE: express inputDrive in terms of dB
        inputDrive = drive * 36.0f - 18.0f;

        //now convert dB to Amp
        inputDrive = powf(10, inputDrive/20);
    }
    
    void setOutputLevel(float level)
    {
        //NOTE:express outputLevel in terms of dB
        outputLevel = level * 36.0f - 18.0f;

        //now convert dB to Amp
        outputLevel = powf(10, outputLevel/20);
    }
    
    
    void setAudioUnitParameters(AUParameter param, float paramLevel)
    {
        switch(param)
        {
            case eSaturationDrive: inSaturation->setDrive(paramLevel); break;
            case eSaturationOuput: inSaturation->setOutput(paramLevel); break;
            case eSaturationRate: inSaturation->setRateOdd(paramLevel); break;
            case eSaturationThresh: inSaturation->setThreshold(paramLevel); break;
            case eSaturationGlobalLevel: inSaturation->setGlobalLevel(paramLevel); break;
            
            case eShameFreq: shame->setRate(paramLevel); break;
            case eShameDepth: shame->setDepth(paramLevel); break;
            case eShameGlobalLevel: shame->setInterpolatedParameters(paramLevel); break;
                
            case eHurricaneSandyGlobalLevel: hurricaneSandy->setInterpolatedParameters(paramLevel); break;
                
            case eHissLevel:   hiss->setHissLevel(paramLevel); break;
                
            case eBlendLevel:  blend->setBlendLevel(paramLevel); break;
                
            case eFlangeDepth:  flange->setDepth(paramLevel); break;
                
            case eBypass:      bypassGraph = paramLevel; break;
            case eInputDrive:  setInputDrive(paramLevel); break;
            case eOutputLevel: setOutputLevel(paramLevel); break;
                
            default: break;
        }
    }
    
    float getOutputLevel(){return outputLevel;}
    
    bool isGraphBypassed(){return bypassGraph;}
    
private:
    
    EShameEnvironments currentEnvironment;
    
    AudioSampleBuffer audioGraphProcessingBuffer;
    
    std::unique_ptr<Hiss> hiss;
    std::unique_ptr<Shame> shame;
    std::unique_ptr<InputSaturation> inSaturation;
    std::unique_ptr<Blend> blend;
    std::unique_ptr<Flange> flange;
    
    
    std::unique_ptr<HurricaneSandy> hurricaneSandy;
    
    bool bypassGraph;
    
    float inputDrive;
    float outputLevel;
    
};


#endif /* defined(__KissOfShame__AudioGraph__) */
