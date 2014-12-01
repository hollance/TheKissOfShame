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
    
    InputSaturation(float threshold, float rateOdd, float rateEven)
    {
        drive = 0.0;
        output = 1.0;
        
        if(threshold > 1.0)      satThreshold = 1.0;
        else if(threshold < 0.0) satThreshold = 0.0;
        else                     satThreshold = threshold;

        satRateOdd = (rateOdd < 0.0) ? 0.0 : rateOdd;
        satRateEven = (rateEven < 0.0) ? 0.0 : rateEven;
        
        saturationGlobalLevel = 0.0;
        
        oddGain = 1.0;
        evenGain = 0.3;
        
        for(int i = 0; i < 10; i++)
        {
            priorSamp[i] = 0.0;
        }
        coef = 0.0;
        setFrequencyRolloff(4000);
    }
    
    ~InputSaturation(){}
    
    
    void setRateOdd(float rate)
    {
        satRateOdd = (rate < 0.0) ? 0.0 : rate;
    }
    void setRateEven(float rate)
    {
        satRateEven = (rate < 0.0) ? 0.0 : rate;
    }

    
    void setThreshold(float thresh)
    {
        if(thresh > 1.0)      satThreshold = 1.0;
        else if(thresh < 0.0) satThreshold = 0.0;
        else                  satThreshold = thresh;
    }
    
    void setDrive(float _drive)
    {
        //NOTE: drive input is in dB
        drive = _drive * 36.0 - 18.0;
        
        //now convert dB to Amp
        drive = powf(10, drive/20);
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
        sampleBufferEven = sampleBuffer;
        
        //odd harmonic distortion saturation
        processOddHarmonicWaveshaping(sampleBuffer, numChannels);
        sampleBuffer.applyGain(oddGain);
 
        //even harmonic distortion saturation
        processEvenHarmonicWaveshaping(sampleBufferEven, numChannels);
        sampleBufferEven.applyGain(evenGain);
        
        //Mix the even and odd harmonic signals
        weightEvenAndOddWaveshaping(sampleBuffer, sampleBufferEven, numChannels);
        
        //apply single-pole lowpass filter, rolloff at 4000Hz.
        lowPassFilter(sampleBuffer, numChannels);
     };
    
    
    void lowPassFilter(AudioSampleBuffer& sampleBuffer, int numChannels)
    {
        for (int channel = 0; channel < numChannels; ++channel)
        {
            float last = priorSamp[channel]; //Needs to be specific to channel!!!
            float feedback = 1 - coef;
            float* sample = sampleBuffer.getWritePointer(channel);
            
            for (int i = 0; i < sampleBuffer.getNumSamples(); i++)
            {
                last = sample[i] = coef * sample[i] + feedback * last;
            }
            
            priorSamp[channel] = last;
        }
    }
    
    void weightEvenAndOddWaveshaping(AudioSampleBuffer& sampleBufferOdd, AudioSampleBuffer& sampleBufferEven, int numChannels)
    {
        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* sample_odd = sampleBufferOdd.getWritePointer(channel);
            float* sample_even = sampleBufferEven.getWritePointer(channel);
            
            for(int i = 0; i < sampleBufferOdd.getNumSamples(); i++)
            {
                sample_odd[i] = (oddGain*sample_odd[i] + evenGain*sample_even[i])/(oddGain + evenGain);
            }
        }
    }
    
    void processEvenHarmonicWaveshaping(AudioSampleBuffer& sampleBuffer, int numChannels)
    {
        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* samples = sampleBuffer.getWritePointer(channel);
            
            for(int i = 0; i < sampleBuffer.getNumSamples(); i++)
            {
                samples[i] = fabs(samples[i]);
                samples[i] *= drive;
                
                samples[i] = tanhf(satRateEven * samples[i]);
                
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
                    samples[i] = satThreshold + tanhf(satRateOdd * (fabs(samples[i]) - satThreshold)) * (1.0 - satThreshold);
                }
                else if(samples[i] < -satThreshold)
                {
                    samples[i] = -satThreshold - tanhf(satRateOdd * (fabs(samples[i]) - satThreshold)) * (1.0 - satThreshold);
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
    
    float satRateEven;
    float satRateOdd;          //How quickly the saturation approaches the upper bound.
    float satThreshold;     //The amplitude level at which the saturation waveshaping begins.
    
    float drive;
    float output;
    
    float saturationGlobalLevel;
    
    AudioSampleBuffer sampleBufferEven;
    
    //For single-pole low pass filter
    float priorSamp[10];
    float coef;
    
};


#endif /* defined(__KissOfShame__InputSaturation__) */
