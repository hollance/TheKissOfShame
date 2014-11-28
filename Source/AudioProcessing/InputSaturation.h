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
        drive = 0.0;
        output = 1.0;
        
        if(threshold > 1.0)      satThreshold = 1.0;
        else if(threshold < 0.0) satThreshold = 0.0;
        else                     satThreshold = threshold;

        satRate = (rate < 0.0) ? 0.0 : rate;
        
        saturationGlobalLevel = 0.0;
        
        oddGain = 1.0;
        evenGain = 1.0;
        
        priorSamp = 0;
        coef = 0;
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
    
    
    void setFrequencyRolloff(float f)
    {
        if (f < 0) f = 0;
        
        coef = f * (2 * PI) / SAMPLE_RATE;
        if (coef > 1)
            coef = 1;
        else if (coef < 0)
            coef = 0;
    }
    
    void processInputSaturation(AudioSampleBuffer& sampleBuffer, int numChannels)
    {
        
        sampleBufferCopy = sampleBuffer;
        
        //odd harmonic distortion saturation
        processOddHarmonicWaveshaping(sampleBuffer, numChannels);
        sampleBuffer.applyGain(oddGain);
 
        //even harmonic distortion saturation
        processEvenHarmonicWaveshaping(sampleBufferCopy, numChannels);
        sampleBufferCopy.applyGain(evenGain);
        
        //Mix the even and odd harmonic signals
        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* sample_odd = sampleBuffer.getWritePointer(channel);
            float* sample_even = sampleBufferCopy.getWritePointer(channel);
            
            for(int i = 0; i < sampleBuffer.getNumSamples(); i++)
            {
                sample_odd[i] = oddGain*sample_odd[i] + evenGain*sample_even[i]/(oddGain + evenGain);
            }
        }
        
        //apply single-pole lowpass filter, rolloff at 4000Hz.
        float last = priorSamp;
        float feedback = 1 - coef;
        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* sample = sampleBuffer.getWritePointer(channel);
            
            for(int i = 0; i < sampleBuffer.getNumSamples(); i++)
            {
                for (int i = 0; i < sampleBuffer.getNumSamples(); i++)
                    last = sample[i] = coef * sample[i] + feedback * last;
                
                priorSamp = last;
            }
        }
    };
    
    
    void processEvenHarmonicWaveshaping(AudioSampleBuffer& sampleBuffer, int numChannels)
    {
        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* samples = sampleBuffer.getWritePointer(channel);
            
            for(int i = 0; i < sampleBuffer.getNumSamples(); i++)
            {
                samples[i] = fabs(samples[i]);
                samples[i] *= drive;
                
                if(samples[i] > satThreshold)
                {
                    samples[i] = satThreshold + tanhf(satRate * (samples[i] - satThreshold)) * (1.0 - satThreshold);
                }
                else
                {
                    //samples[i] = samples[i];
                }
                
                samples[i] *= output;
            }
        }
    }
    
    void processOddHarmonicWaveshaping(AudioSampleBuffer& sampleBuffer, int numChannels)
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
    }



    
private:
    
    float evenGain;
    float oddGain;
    
    float satRate;          //How quickly the saturation approaches the upper bound.
    float satThreshold;     //The amplitude level at which the saturation waveshaping begins.
    float drive;
    float output;
    
    float saturationGlobalLevel;
    
    AudioSampleBuffer sampleBufferCopy;
    
    //For single-pole low pass filter
    float priorSamp;
    float coef;
    
};


#endif /* defined(__KissOfShame__InputSaturation__) */
