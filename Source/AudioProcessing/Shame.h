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
        rate = 2.0;
        depth = 0.5;
        //oscIncr = 0.0;
        
        playPosition = 0.0;
        curPos = 0;
        
        shameGlobalLevel = 0.0;
        
        shameSampleBuffer = new AudioSampleBuffer(numChannels, BUFFER_SIZE);
        
        
        ////////////////initialization for wavetable
        curPos_wTable = 0;
        waveformIndx = 0;
        randPeriodicity = 0.0;
        curDirection = 1.0;
        rateFluctuation = 0.0;
        
        rNum.setSeedRandomly();

        importWaveTables();
    }
    
    ~Shame(){}
    
    void interpolateParameters(float input)
    {
        if(input < 0.0) input = 0.0;
        if(input > 1.0) input = 1.0;
        
        if(input <= 0.3)
        {
            depth = 0.2 * input;
            randPeriodicity = 0.0;
            rate = 75.0;
            waveformIndx = 3;
        }
        else if(input > 0.3 && input <= 0.5)
        {
            depth = 0.2 + 0.1*(input - 0.3);
            randPeriodicity = 0.25*(input - 0.3);
            rate = 75.0;
            waveformIndx = 3;
        }
        else if(input > 0.5 && input <= 1.0)
        {
            depth = 0.3 + 0.7*(input - 0.5);
            randPeriodicity = 0.25 + 0.25*(input - 0.5);
            rate = 75.0 - 40.0*(input - 0.5);
            waveformIndx = 3;
        }
    }
    
    
    void importWaveTables()
    {
        Array<File> audioFiles;
        File audioFile1("/Users/brianhansen/Documents/Brian/Work/1_KOS/kissofshame/Audio_Resources/Hiss.wav");
        File audioFile2("/Users/brianhansen/Documents/Brian/Work/1_KOS/kissofshame/Audio_Resources/Hiss.wav");
        
        audioFiles.add(audioFile1);
        audioFiles.add(audioFile2);
        
        for(int i = 0; i < audioFiles.size(); i++)
        {
            if (!audioFiles[i].existsAsFile()) return;
            
            AudioFormatManager formatManager;
            formatManager.registerBasicFormats();
            AudioFormatReader* reader = formatManager.createReaderFor (audioFiles[i]);
            
            if (!reader) return;

            AudioSampleBuffer *tempAudioBuffer = new AudioSampleBuffer(reader->numChannels, (int) reader->lengthInSamples);
            reader->read(tempAudioBuffer, 0, 44100, 0, true, true);
            
            delete reader;
            
            waveTableBuffers.add(tempAudioBuffer);
        }
    }
    
    
    float linearInterpolate(float prSamp, float nxtSamp, float fraction)
    {
        return prSamp*(1-fraction) + nxtSamp*fraction;
    }
    
    
    float processWavetable()
    {
        //indexing logic for the wavetables
        int prWaveIndx = 0;
        int nxtWaveIndx = 0;
        
        if(waveformIndx < 0)
        {
            prWaveIndx = 0;
            nxtWaveIndx = 0;
            waveformIndx = 0;
        }
        else if(waveformIndx >= 0 && waveformIndx < 1)
        {
            prWaveIndx = 0;
            nxtWaveIndx = 1;
        }
        else if(waveformIndx >= 1 && waveformIndx < 2)
        {
            prWaveIndx = 1;
            nxtWaveIndx = 2;
        }
        else if(waveformIndx >= 2 && waveformIndx < 3)
        {
            prWaveIndx = 2;
            nxtWaveIndx = 3;
        }
        else if(waveformIndx >= 3)
        {
            prWaveIndx = 3;
            nxtWaveIndx = 3;
            waveformIndx = 3;
        }
        float fracWaveIndx = waveformIndx - (int)waveformIndx;
        
        
        //interpolation indices for the samples in each wavetable
        float fracPos = curPos_wTable - (int)curPos_wTable;
        int prPos = (int)curPos_wTable;
        int nxtPos = (prPos + 1) % BUFFER_SIZE;
        
        float w1Sample = waveTableBuffers[prWaveIndx]->getReadPointer(0)[prPos]*(1-fracPos) + waveTableBuffers[prWaveIndx]->getReadPointer(0)[nxtPos]*fracPos;
        float w2Sample = waveTableBuffers[nxtWaveIndx]->getReadPointer(0)[prPos]*(1-fracPos) + waveTableBuffers[nxtWaveIndx]->getReadPointer(0)[nxtPos]*fracPos;
        
        float outsample = w1Sample*(1-fracWaveIndx) + w2Sample*fracWaveIndx;
        
        
        //randomize the periodicity.
        if((outsample < 0 && curDirection > 0) || (outsample > 0 && curDirection < 0))
        {
            curDirection *= -1;
            rateFluctuation = ((float)rNum.nextInt(2000)/1000 - 1.0)*rate*randPeriodicity;
            //post("Random Number Triggered: %f", rateFluctuation);
        }
        
        //increment the current position.
        curPos_wTable = curPos_wTable + rate + rateFluctuation;
        if(curPos_wTable >= BUFFER_SIZE) curPos_wTable -= BUFFER_SIZE;
        
        
        return outsample;
    }

    
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
            
            //float oscVal = 1.0 - oscDepth*sin(oscFreq*2*PI*(oscIncr/SAMPLE_RATE));
            float oscVal = 1.0 - depth*processWavetable();
            //oscIncr = ((int)oscIncr + 1) % (int)SAMPLE_RATE;
            playPosition += oscVal;
        }
    };

    
    void setRate(float _rate){rate = _rate;}
    void setDepth(float _depth){depth = _depth;}
    void setPeriodicity(float periodicity){randPeriodicity = periodicity;}
    void setWaveformIndex(float indx){waveformIndx = indx;}
    void setGlobalLevel(float level){shameGlobalLevel = level;}

    
private:
    
    ScopedPointer<AudioSampleBuffer> shameSampleBuffer;
    
    float shameGlobalLevel;
    
    float playPosition;
    int curPos;
    
    //float rate;
    float depth;
    //float oscIncr;
    
    ///////// controls for the waveform oscillator
    
    //used for core indexing of the waveform.
    OwnedArray<AudioSampleBuffer> waveTableBuffers;
    float waveformIndx;
    float curPos_wTable;
    float rate;
    
    //used for randomization of waveform reading.
    Random rNum;
    float randPeriodicity;
    int curDirection;
    float rateFluctuation;
    
};


#endif /* defined(__KissOfShame__Shame__) */
