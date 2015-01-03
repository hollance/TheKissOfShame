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
        curDepth = 0.0;
        targetDepth = 0.0;
        
        playPosition = 0.0;
        curPos = 0;
        
        flangeSampleBuffer = new AudioSampleBuffer(numChannels, FLANGE_BUFF_SIZE);
     }
    
    ~Flange(){}
    
    
    float linearInterpolate(float prSamp, float nxtSamp, float fraction)
    {
        return prSamp*(1-fraction) + nxtSamp*fraction;
    }
    
    double cubicInterpolate(double prSamp2, double prSamp, double nxtSamp, double nxtSamp2, double fraction)
    {
        double a0, a1, a2, a3, fracSqared;
        
        fracSqared = fraction*fraction;
        
//        a0 = nxtSamp2 - nxtSamp1 - prSamp2 + prSamp1;
//        a1 = prSamp2 - prSamp1 - a0;
//        a2 = nxtSamp1 - prSamp2;
//        a3 = prSamp1;
        
        a0 = -0.5*prSamp2 + 1.5*prSamp - 1.5*nxtSamp + 0.5*nxtSamp2;
        a1 = prSamp2 - 2.5*prSamp + 2*nxtSamp - 0.5*nxtSamp2;
        a2 = -0.5*prSamp2 + 0.5*nxtSamp;
        a3 = prSamp;

        return (a0*fraction*fracSqared + a1*fracSqared + a2*fraction + a3);
    }
    
    void processFlange(AudioSampleBuffer& sampleBuffer, int numChannels)
    {
        
        for(int i = 0; i < sampleBuffer.getNumSamples(); i++)
        {
            //populate the circular buffer
            for(int channel = 0; channel < numChannels; ++channel)
            {
                float curSample = sampleBuffer.getReadPointer(channel)[i];
                flangeSampleBuffer->getWritePointer(channel)[curPos] = curSample;
            }
            
            
            float frac = playPosition - (int)playPosition;
            
            int prX = (int)playPosition;
            int nxtX = (prX + 1) % FLANGE_BUFF_SIZE;
            
            //int prX2 = prX - 1;
            //if(prX2 < 0) prX2 += FLANGE_BUFF_SIZE;
            //int nxtX2 = (nxtX + 1) % FLANGE_BUFF_SIZE;
            
            //compute the output sample by indexing and interpolating the circular buffer.
            for(int channel = 0; channel < numChannels; ++channel)
            {
                float* outSamples = sampleBuffer.getWritePointer(channel);
                outSamples[i] = 0.5*outSamples[i] + 0.5*linearInterpolate(flangeSampleBuffer->getReadPointer(channel)[prX], flangeSampleBuffer->getReadPointer(channel)[nxtX],frac);
               // outSamples[i] = 0.5*outSamples[i] + 0.5*cubicInterpolate(flangeSampleBuffer->getReadPointer(channel)[prX2],flangeSampleBuffer->getReadPointer(channel)[prX], flangeSampleBuffer->getReadPointer(channel)[nxtX], flangeSampleBuffer->getReadPointer(channel)[nxtX2], frac);
            }
            
            if(fabs(targetDepth - curDepth) < .01)
                curDepth = targetDepth;
            else
                curDepth += (targetDepth - curDepth)*.001;
            
            playPosition = curPos - curDepth;
            
            if(playPosition >= FLANGE_BUFF_SIZE) playPosition -= FLANGE_BUFF_SIZE;
            if(playPosition < 0) playPosition += FLANGE_BUFF_SIZE;

            
            curPos = (curPos + 1) % FLANGE_BUFF_SIZE;
        }
    };

//NOTE: this process routine uses linear interpolation
//    void processFlange(AudioSampleBuffer& sampleBuffer, int numChannels)
//    {
//        
//        for(int i = 0; i < sampleBuffer.getNumSamples(); i++)
//        {
//            //populate the circular buffer
//            for(int channel = 0; channel < numChannels; ++channel)
//            {
//                float curSample = sampleBuffer.getReadPointer(channel)[i];
//                flangeSampleBuffer->getWritePointer(channel)[curPos] = curSample;
//            }
//            
//            if(playPosition >= FLANGE_BUFF_SIZE) playPosition -= FLANGE_BUFF_SIZE;
//            if(playPosition < 0) playPosition += (FLANGE_BUFF_SIZE);
//            
//            float frac = playPosition - (int)playPosition;
//            int prX = (int)playPosition;
//            int nxtX = (prX + 1) % FLANGE_BUFF_SIZE;
//            
//            //compute the output sample by indexing and interpolating the circular buffer.
//            for(int channel = 0; channel < numChannels; ++channel)
//            {
//                float* outSamples = sampleBuffer.getWritePointer(channel);
//                outSamples[i] = 0.5*outSamples[i] + 0.5*linearInterpolate(flangeSampleBuffer->getReadPointer(channel)[prX], flangeSampleBuffer->getReadPointer(channel)[nxtX], frac);
//            }
//            
//            playPosition = curPos - depth;
//            
//            curPos = (curPos + 1) % FLANGE_BUFF_SIZE;
//        }
//    };

    
    void setDepth(float _depth){targetDepth = _depth*1000;} //Note input value _depth will be a value between 0 and 1.

    
private:
    
    ScopedPointer<AudioSampleBuffer> flangeSampleBuffer;
    
    float playPosition;
    int curPos;
    
    float curDepth;
    float targetDepth;
    
};


#endif /* defined(__KissOfShame__Shame__) */
