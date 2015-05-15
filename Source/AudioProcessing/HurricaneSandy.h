//
//  Blend.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/14/14.
//
//

#ifndef __KissOfShame__HurricaneSandy__
#define __KissOfShame__HurricaneSandy__


#include "../shameConfig.h"
#include "EnvelopeDips.h"

class HurricaneSandy
{
public:
    
    HurricaneSandy()
    {
        dips = new EnvelopeDips();
        dips->setDomainMS(1000); //NOTE: domain is set in milleseconds
        dips->setDynamicExtremity(0.5);
        dips->setNumPoints(15);
        dips->setNumPointRandomness(0.5);
        dips->calculateDipPoints();
    }
    
    void processHurricaneSandy(AudioSampleBuffer &buffer, int numChannels)
    {
        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* samples = buffer.getWritePointer(channel);
            
            for(int i = 0; i < buffer.getNumSamples(); i++)
            {
                //periodic white noise burst
                samples[i] = buffer.getReadPointer(channel)[i];
                
                //low freq granulated noise ()
                
                
                //filtered granulated noise (amount added is based on rms * noise)
                
                
                //amplitude dips
                samples[i] = dipsLevel*dips->processEnvelopeDips()*samples[i] + (1-dipsLevel)*samples[i];
            }
        }
    }
    
    float getDipsLevel(){return dipsLevel;}
    float getLFNoiseLevel(){return lFNoiseLevel;}
    float getHissLevel(){return hissLevel; }
    float getGrainNoiseLevel(){return grainNoiseLevel; }
    float getNoiseBurstLevel(){return noiseBurstLevel; }
    
    void setDipsLevel(float dE) {dipsLevel = dE;}
    void setLFNoiseLevel(float lFE) {lFNoiseLevel = lFE;}
    void setHissLevel(float hE) {hissLevel = hE;}
    void setGrainNoiseLevel(float gE) {grainNoiseLevel = gE;}
    void setNoiseBurstLevel(float nBE) {noiseBurstLevel = nBE;}
    
    
    void setInterpolatedParameters(float input)
    {
        if(input < 0.0) input = 0.0;
        if(input > 1.0) input = 1.0;
        
        dipsLevel = input;
        lFNoiseLevel = input;
        hissLevel = input;
        grainNoiseLevel = input;
        noiseBurstLevel = input;

//Interpolation example from Shame
//        if(input <= 0.5)
//        {
//            depth = 5 * input / 0.5;
//            randPeriodicity = 0.5 ;
//            rate = 7.0;
//            waveformIndx = 0;
//        }
//        else if(input > 0.5 && input <= 0.85)
//        {
//            depth = 5 + 25*(input - 0.5)/(0.85 - 0.5);
//            randPeriodicity = 0.5 - 0.25*(input - 0.5)/(0.85 - 0.5);
//            rate = 7.0 + 70.0*(input - 0.5)/(0.85 - 0.5);
//            waveformIndx = 0;
//        }
//        else if(input > 0.85 && input <= 1.0)
//        {
//            depth = 30 + 30*(input - 0.85)/0.15;
//            randPeriodicity = 0.25 + 0.5*(input - 0.85)/0.15;
//            rate = 77.0 - 20.0*(input - 0.85)/0.15;
//            waveformIndx = 0;
//        }
    }

    
    
    
private:
    
    ScopedPointer<EnvelopeDips> dips;
    
    float dipsLevel;
    float lFNoiseLevel;
    float hissLevel;
    float grainNoiseLevel;
    float noiseBurstLevel;
};



#endif /* defined(__KissOfShame__HurricaneSandy__) */
