//
//  Shame.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/9/14.
//
//

#ifndef __KissOfShame__Shame__
#define __KissOfShame__Shame__


#include "../shameConfig.h"


class Shame
{
public:
    Shame(int numChannels) //NOTE: number of channels should be in the constructor input.
    {
        oscFreq = 2.0;
        oscDepth = 0.5;
        oscIncr = 0.0;
        
        playPosition = 0.0;
        curPos = 0;
        
        shameGlobalLevel = 0.0;
        
        shameSampleBuffer = new AudioSampleBuffer(numChannels, BUFFER_SIZE);
    }
    
    ~Shame(){}
    
    float linearInterpolate(float prSamp, float nxtSamp, float fraction)
    {
        return prSamp*(1-fraction) + nxtSamp*fraction;
    }
    
    void setOscFreq(float freq){oscFreq = 15.0*freq;}
    void setOscDepth(float depth){oscDepth = depth;}
    void setGlobalLevel(float level){shameGlobalLevel = level;}
    
    
    void processShame(AudioSampleBuffer& sampleBuffer, int numChannels)
    {
        
        for(int i = 0; i < sampleBuffer.getNumSamples(); i++)
        {
            //populate the circular buffer
            for(int channel = 0; channel < numChannels; ++channel)
            {
                float curSample = sampleBuffer.getReadPointer(channel)[i];
                shameSampleBuffer->getWritePointer(channel)[curPos] = curSample;
            }
            
            if(playPosition > BUFFER_SIZE) playPosition -= BUFFER_SIZE;
            
            float frac = playPosition - (int)playPosition;
            int prX = (int)playPosition;
            int nxtX = (prX + 1) % BUFFER_SIZE;
            
            //compute the output sample by indexing and interpolating the circular buffer.
            for(int channel = 0; channel < numChannels; ++channel)
            {
                float* outSamples = sampleBuffer.getWritePointer(channel);
                outSamples[i] = linearInterpolate(shameSampleBuffer->getReadPointer(channel)[prX], shameSampleBuffer->getReadPointer(channel)[nxtX], frac);
            }
            
            curPos = (curPos + 1) % BUFFER_SIZE;
            
            float oscVal = 1.0 - oscDepth*sin(oscFreq*2*PI*(oscIncr/SAMPLE_RATE));
            oscIncr = ((int)oscIncr + 1) % (int)SAMPLE_RATE;
            playPosition += oscVal;
        }
    };

    

    
private:
    
    ScopedPointer<AudioSampleBuffer> shameSampleBuffer;
    
    float playPosition;
    int curPos;
    
    float oscFreq;
    float oscDepth;
    float oscIncr;
    
    float shameGlobalLevel;
};


#endif /* defined(__KissOfShame__Shame__) */
