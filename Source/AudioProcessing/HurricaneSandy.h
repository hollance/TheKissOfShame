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
#include "Granulate.h"
#include "Loop.h"
//#include "Hiss.h"
#include "LoopCrossfade.h"
#include "Biquads.h"
#include "Envelope.h"
#include "Noise.h"

class HurricaneSandy
{
public:
    
    HurricaneSandy() : grainImpact(0.0), ampFluctuationImpact(0.0)
    {
        
        /////////////////////// used for general amplitude fluctuation
        dips = new EnvelopeDips();
        dips->setDomainMS(1000); //NOTE: domain is set in milleseconds
        dips->setDynamicExtremity(0.5);
        dips->setNumPoints(15);
        dips->setNumPointRandomness(0.5);
        dips->calculateDipPoints();
        
        
        ////////////////////// used to mix noise burst
        whiteNoise = new Noise();
        
        noiseEnv = new Envelope(350);
        noiseEnv->addEvelopePoint(0.143, 0.073);
        noiseEnv->addEvelopePoint(0.305, 0.367);
        noiseEnv->addEvelopePoint(0.383, 0.567);
        noiseEnv->addEvelopePoint(0.428, 0.0);
        
        sigEnv = new Envelope(350);
        sigEnv->addEvelopePoint(0.143, 0.2);
        sigEnv->addEvelopePoint(0.305, 0.8);
        sigEnv->addEvelopePoint(0.383, 0.5);
        sigEnv->addEvelopePoint(0.428, 0.0);
        
        
        ////////////////////// used to modulate the amplitude of the incomming signal
        int numVoices = 5;
        granulator = new Granulate(numVoices, AUDIO_PATH + "PinkNoise.wav");
        granulator->setRandomFactor(1.0);
        granulator->setStretch(0);
        granulator->setGrainParameters(5, 50, 50, 50);
        granulator->setVoices(10);
        
        
        ///////////////////// pre recorded low frequency granular noise
        lowFreqGranular = new LoopCrossfade(AUDIO_PATH + "LowLevelGrainNoise.wav");
        lowFreqGranular->setLoopCrossfadeLevel(0.25);
        
        
        ////////////////////  filters for granulation and the main signal
        lpButterworth_Grains = new Biquads;
        lpButterworth_Grains->setButterworth_LowHighPass(2000, 1, true);
        
        hpButterworth_Grains = new Biquads;
        hpButterworth_Grains->setButterworth_LowHighPass(50, 1, false);
        
        lpButterworth_Signal = new Biquads;
        lpButterworth_Signal->setButterworth_LowHighPass(22050, 1, true);
        
        
        //initialize parameters
        setInterpolatedParameters(0.0);

    }
    
    //Needs:
    //Filtering!!!!
    //- LP on dry signal.
    //- Buttorsworth LP + HP on granulated noise
    //Ducking:
    //- dry signal ducks in sync with noise burst
    //Reverb:
    //Maybe just a little reverb to blend the sounds... 
    
    
    void processHurricaneSandy(AudioSampleBuffer &buffer, int numChannels)
    {
        
        for(int i = 0; i < buffer.getNumSamples(); i++)
        {
            
            float lfGrainSample = lowFreqGranular->processLoopCrossSample(0);
            float grainSample = granulator->tick();
            
            grainSample = lpButterworth_Grains->process(grainSample, 0);
            grainSample = hpButterworth_Grains->process(grainSample, 0);
            
            
            float dipsLevel = dips->processEnvelopeDips();
            
            float noiseBurstEnvValue = noiseEnv->processEnvelope();
            float signalEnvValue = (1 - sigEnv->processEnvelope());
            
            
            for (int channel = 0; channel < numChannels; ++channel)
            {
                float* samples = buffer.getWritePointer(channel);
                //float bufferMagnitude = buffer.getMagnitude(channel, 0, buffer.getNumSamples());
                //float scaledGrainSample = grainSample*bufferMagnitude;
                
                //lowpass filter the original signal
                if(channel < 2)
                    samples[i] = lpButterworth_Signal->process(samples[i], channel);
                
                //mix signal with periodic noise burst.
                samples[i] = (1 - noiseBurstImpact)*samples[i] + noiseBurstImpact*(signalEnvValue*samples[i] + 0.05*noiseBurstEnvValue*whiteNoise->tick());

                //Modulate the amplitude by the granular amplitude
                samples[i] *= (1 - grainImpact*grainSample);

                
                //Fluctuate the overall amplitude
                samples[i] = ampFluctuationImpact*dipsLevel*samples[i] + (1 - ampFluctuationImpact)*samples[i];
                
                
                //mix signal with all of the low freq grain noise.
                samples[i] = samples[i] + lowFreqGrainNoiseLevel*lfGrainSample;  //(1 - lowFreqGrainNoiseLevel)*NOTE: orig mult sample[i]
             }
        }
    }
    
    
    void setInterpolatedParameters(float input)
    {
        if(input < 0.0) input = 0.0;
        if(input > 1.0) input = 1.0;
        
        //grain interpolation
        grainImpact = 1.0 * input;
        granulator->setGrainParameters(10*input + 5, 75, 50, 1000*(1.01 - input));

        //amount of low frequency granular noise mixed in.
        lowFreqGrainNoiseLevel = 0.15 * input;
        
        //Amplitude fluctuation
        ampFluctuationImpact = input;
        
        //Amplitude fluctuation
        if(input > 0.5)
            noiseBurstImpact = 2*(input - 0.5);
        
        //low pass filtering of original signal
        lpButterworth_Signal->setButterworth_LowHighPass(20050*(1 - input) + 2000, 1, true);
    }

    
    
    
private:
    
    ScopedPointer<EnvelopeDips> dips;
    ScopedPointer<Granulate> granulator;
    ScopedPointer<LoopCrossfade> lowFreqGranular;
    ScopedPointer<Biquads> lpButterworth_Grains;
    ScopedPointer<Biquads> hpButterworth_Grains;
    ScopedPointer<Biquads> lpButterworth_Signal;
    
    ScopedPointer<Noise> whiteNoise;
    ScopedPointer<Envelope> noiseEnv;
    ScopedPointer<Envelope> sigEnv;
    
    
    float grainImpact;
    float lowFreqGrainNoiseLevel;
    float ampFluctuationImpact;
    float noiseBurstImpact;
    
};



#endif /* defined(__KissOfShame__HurricaneSandy__) */
