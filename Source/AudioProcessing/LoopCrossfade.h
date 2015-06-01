//
//  Hiss.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/9/14.
//
//

#ifndef __KissOfShame__LoopCrossfade__
#define __KissOfShame__LoopCrossfade__

#include <iostream>

#include "../shameConfig.h"

class LoopCrossfade
{
public:
    
    LoopCrossfade(String filePath)
    {
        File audioFile(filePath);
        LoopCrossfadeBuffer = new AudioSampleBuffer();
        if(audioFile.existsAsFile()) LoopCrossfadeBuffer = loadSampleFromFile(audioFile);
                
        setLoopCrossfadeLevel(0.0);
        
        indx1 = 0;
        indx2 = LoopCrossfadeBuffer->getNumSamples()/2;
        
//        crossfade = false;
    }
    
    ~LoopCrossfade(){}
    
    void setAudioFile(String audioFilePath)
    {
        LoopCrossfadeBuffer->clear();
        
        File audioFile(audioFilePath);
        LoopCrossfadeBuffer = new AudioSampleBuffer();
        if(audioFile.existsAsFile()) LoopCrossfadeBuffer = loadSampleFromFile(audioFile);
    }

    
    
//    void chooseFileToOpen()
//    {
//        FileChooser selectFile(T("Choose a file to open"));
//        if(selectFile.browseForFileToOpen())
//        {
//            currentFile = selectFile.getResult();
//            
//            //load into transport for playback
//            loadFileIntoAudioTransport(currentFile);
//            
//            //load sample point buffer.
//            AudioSampleBuffer *as = loadSampleFromFile(currentFile);
//            cout << "Number of samples in Buffer: " << as->getNumSamples() << endl;
//            cout << "Number of channels in Buffer: " << as->getNumChannels() << endl;
//            //        for(int i = 0; i < as->getNumSamples(); i++){
//            //            cout << "Channel 0. Audio Sample " << i << ": " << *(as->getSampleData(0,i)) << endl;
//            //        }
//            
//        }
//    }

    
    AudioSampleBuffer *loadSampleFromFile(const File& audioFile)
    {
        AudioSampleBuffer *toneASB = NULL;
        //    File noteFile(File::getCurrentWorkingDirectory().getChildFile(fileName));
        if (audioFile.existsAsFile()) {
            AudioFormatManager formatManager;
            formatManager.registerBasicFormats();
            
            AudioFormatReader* reader = formatManager.createReaderFor (audioFile);
            
            if (reader) {
                toneASB = new AudioSampleBuffer(reader->numChannels, (int) reader->lengthInSamples);
                //toneASB->readFromAudioReader(reader, 0, (int) reader->lengthInSamples, 0, true, true);
                reader->read(toneASB, 0, (int) reader->lengthInSamples, 0, true, true);
            }
            delete reader;
        }
        return toneASB;
    }

    
    void processLoopCrossfade(AudioSampleBuffer& sampleBuffer, int numChannels)
    {
        for(int i = 0; i < sampleBuffer.getNumSamples(); i++)
        {
            if(indx1 < 0.5*LoopCrossfadeBuffer->getNumSamples())
                rampValue = (float)indx1 / (0.5*(float)LoopCrossfadeBuffer->getNumSamples());
            else
                rampValue = (float)(LoopCrossfadeBuffer->getNumSamples() - indx1) / (0.5*(float)LoopCrossfadeBuffer->getNumSamples());


            for(int channel = 0; channel < numChannels; ++channel)
            {
                float* samples = sampleBuffer.getWritePointer(channel);
                
                loopCrossfadeSample = rampValue*LoopCrossfadeBuffer->getReadPointer(channel)[indx1] + (1-rampValue)*LoopCrossfadeBuffer->getReadPointer(channel)[indx2];
                
                samples[i] = signalLevel*samples[i] + loopCrossfadeLevel*loopCrossfadeSample;
            }
            
            
            indx1 = (indx1 + 1) % LoopCrossfadeBuffer->getNumSamples();
            //if(crossfade) indx2 = (indx2 + 1) % hissBuffer->getNumSamples();
            indx2 = (indx2 + 1) % LoopCrossfadeBuffer->getNumSamples();
        }
    }
    
    float processLoopCrossSample(int channel)
    {
        
        if(indx1 < 0.5*LoopCrossfadeBuffer->getNumSamples())
            rampValue = (float)indx1 / (0.5*(float)LoopCrossfadeBuffer->getNumSamples());
        else
            rampValue = (float)(LoopCrossfadeBuffer->getNumSamples() - indx1) / (0.5*(float)LoopCrossfadeBuffer->getNumSamples());
        
        loopCrossfadeSample = rampValue*LoopCrossfadeBuffer->getReadPointer(channel)[indx1] + (1-rampValue)*LoopCrossfadeBuffer->getReadPointer(channel)[indx2];
        
        
        indx1 = (indx1 + 1) % LoopCrossfadeBuffer->getNumSamples();
        //if(crossfade) indx2 = (indx2 + 1) % hissBuffer->getNumSamples();
        indx2 = (indx2 + 1) % LoopCrossfadeBuffer->getNumSamples();
        
        return loopCrossfadeLevel*loopCrossfadeSample;
    }

    
    
    void setLoopCrossfadeLevel(float level)
    {
        loopCrossfadeLevel = level;
        signalLevel = 1 - loopCrossfadeLevel;
        
//        signalLevel = 0.0 - 3.0*level; //reducing signal level by this much (up to 3dB)
//        hissLevel = 3.0*level - 36.0; //replacing with hiss.
//        
//        signalLevel = powf(10, signalLevel/20); //convert to amp
//        hissLevel = powf(10, hissLevel/20); //convert to amp
    }
    
    
    
//    void setInputDrive(float drive)
//    {
//        //NOTE: drive input is in dB
//        inputDrive = drive * 36.0 - 18.0;
//        
//        //now convert dB to Amp
//        inputDrive = powf(10, inputDrive/20);
//    }

    
    
private:
    
    ScopedPointer<AudioSampleBuffer> LoopCrossfadeBuffer;
    int indx1;
    int indx2;
    float loopCrossfadeSample;
    
    float loopCrossfadeLevel;
    float signalLevel;
    
    
    float rampValue;
    
//    bool crossfade;
    
};

#endif /* defined(__KissOfShame__Hiss__) */
