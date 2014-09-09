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


class InputSaturation
{
public:
    
    InputSaturation(float threshold, float rate)
    {
        drive = 1.0;
        output = 1.0;
        
        if(threshold > 1.0)      satThreshold = 1.0;
        else if(threshold < 0.0) satThreshold = 0.0;
        else                     satThreshold = threshold;

        satRate = (rate < 0.0) ? 0.0 : rate;
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
        drive = _drive;
    }
    
    void setOutput(float _output)
    {
        output = _output;
    }
    
    
    float processInputSaturation(float sample)
    {
        sample *= drive;
//        satThreshold = 0.1;
//        satRate = 2.0;
        
        if(sample > satThreshold)
        {
            sample = satThreshold + tanhf(satRate * (fabs(sample) - satThreshold)) * (1.0 - satThreshold);
        }
        else if(sample < -satThreshold)
        {
            sample = -satThreshold - tanhf(satRate * (fabs(sample) - satThreshold)) * (1.0 - satThreshold);
        }
        else
        {
            //sample = sample;
        }
        
        return sample *= output;
        
        
    };

    
private:
    
    float satRate;          //How quickly the saturation approaches the upper bound.
    float satThreshold;     //The amplitude level at which the saturation waveshaping begins.
    float drive;
    float output;
};


#endif /* defined(__KissOfShame__InputSaturation__) */
