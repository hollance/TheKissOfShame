//
//  Blend.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/14/14.
//
//

#ifndef __KissOfShame__Blend__
#define __KissOfShame__Blend__


#include "../shameConfig.h"

class Blend
{
public:
    Blend()
    {
        blendValue = 0.0;
    }
    
    void processBlend(AudioSampleBuffer &buffer1, AudioSampleBuffer &buffer2, int numChannels)
    {
        
        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* samples = buffer1.getWritePointer(channel);
            
            for(int i = 0; i < buffer1.getNumSamples(); i++)
            {
                samples[i] = (1-blendValue)*buffer1.getReadPointer(channel)[i] + blendValue*buffer2.getReadPointer(channel)[i];
            }
        }
    }
    
    void setBlendLevel(float level) {blendValue = level;}
    
private:
    
    float blendValue;
    
};



#endif /* defined(__KissOfShame__Blend__) */
