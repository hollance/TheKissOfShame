//
//  Flange.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/9/14.
//
//

#ifndef __KissOfShame__Flange__
#define __KissOfShame__Flange__

#include "../shameConfig.h"

#define FLANGE_BUFF_SIZE 2000

class Flange
{
public:
    
    Flange(int numChannels) //NOTE: number of channels should be in the constructor input.
    {
        depth = 0.0;
        
        playPosition = 0.0;
        curPos = 0;
        
        flangeSampleBuffer = new AudioSampleBuffer(numChannels, FLANGE_BUFF_SIZE);
        
     }
    
    ~Flange(){}
    
    
    float linearInterpolate(float prSamp, float nxtSamp, float fraction)
    {
        return prSamp*(1-fraction) + nxtSamp*fraction;
    }
    
    
    void processShame(AudioSampleBuffer& sampleBuffer, int numChannels)
    {
        
        for(int i = 0; i < sampleBuffer.getNumSamples(); i++)
        {
            //populate the circular buffer
            for(int channel = 0; channel < numChannels; ++channel)
            {
                float curSample = sampleBuffer.getReadPointer(channel)[i];
                flangeSampleBuffer->getWritePointer(channel)[curPos] = curSample;
            }
            
            if(playPosition >= FLANGE_BUFF_SIZE) playPosition -= FLANGE_BUFF_SIZE;
            if(playPosition < 0) playPosition += (FLANGE_BUFF_SIZE);
            
            float frac = playPosition - (int)playPosition;
            int prX = (int)playPosition;
            int nxtX = (prX + 1) % FLANGE_BUFF_SIZE;
            
            //compute the output sample by indexing and interpolating the circular buffer.
            for(int channel = 0; channel < numChannels; ++channel)
            {
                float* outSamples = sampleBuffer.getWritePointer(channel);
                outSamples[i] = 0.5*outSamples[i] + 0.5*linearInterpolate(flangeSampleBuffer->getReadPointer(channel)[prX], flangeSampleBuffer->getReadPointer(channel)[nxtX], frac);
            }
            
            playPosition = curPos - depth;//*processWavetable();
            
            curPos = (curPos + 1) % FLANGE_BUFF_SIZE;
                        
        }
    };

    
    void setDepth(float _depth){depth = _depth;}

    
private:
    
    ScopedPointer<AudioSampleBuffer> flangeSampleBuffer;
    
    float playPosition;
    int curPos;
    
    float depth;
    
};


#endif /* defined(__KissOfShame__Shame__) */
