//
//  InputSaturation.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/9/14.
//
//

#ifndef __KissOfShame__InputSaturation__
#define __KissOfShame__InputSaturation__



#include <iostream>
#include "math.h"

#include "../shameConfig.h"




class InputSaturation
{
public:
    
    InputSaturation(float threshold, float rate)
    {
        drive = 1.0;
        output = 0.0;
        
        if(threshold > 1.0)      satThreshold = 1.0;
        else if(threshold < 0.0) satThreshold = 0.0;
        else                     satThreshold = threshold;

        satRate = (rate < 0.0) ? 0.0 : rate;
        
        saturationGlobalLevel = 0.0;
    }
    
    ~InputSaturation(){}
    
    
    void setRate(float rate)
    {
        satRate = (rate < 0.0) ? 0.0 : rate;
    }
    
    void setThreshold(float thresh)
    {
        if(thresh > 1.0)      satThreshold = 1.0;
        else if(thresh < 0.0) satThreshold = 0.0;
        else                  satThreshold = thresh;
    }
    
    void setDrive(float _drive)
    {
        drive = _drive*5.0;
    }
    
    void setOutput(float _output)
    {
        output = _output;
    }
    
    void setGlobalLevel(float level) {saturationGlobalLevel = level;}
        
    void processInputSaturation(AudioSampleBuffer& sampleBuffer, int numChannels)
    {
        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* samples = sampleBuffer.getWritePointer(channel);
            
            
            for(int i = 0; i < sampleBuffer.getNumSamples(); i++)
            {
                samples[i] *= drive;
                
                if(samples[i] > satThreshold)
                {
                    samples[i] = satThreshold + tanhf(satRate * (fabs(samples[i]) - satThreshold)) * (1.0 - satThreshold);
                }
                else if(samples[i] < -satThreshold)
                {
                    samples[i] = -satThreshold - tanhf(satRate * (fabs(samples[i]) - satThreshold)) * (1.0 - satThreshold);
                }
                else
                {
                    //samples[i] = samples[i];
                }
                
                samples[i] *= output;
            }
        }
    };


    
private:
    
    float satRate;          //How quickly the saturation approaches the upper bound.
    float satThreshold;     //The amplitude level at which the saturation waveshaping begins.
    float drive;
    float output;
    
    float saturationGlobalLevel;
};


#endif /* defined(__KissOfShame__InputSaturation__) */
