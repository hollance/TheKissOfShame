//
//  Loop.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/9/14.
//
//

#ifndef __KissOfShame__Loop__
#define __KissOfShame__Loop__

#include <iostream>
#include "../shameConfig.h"

using namespace juce;

class Loop
{
public:
    Loop(String audioFilePath) : loopDuration(0.0)
    {
        File loopFile(audioFilePath);
        loopBuffer.reset(new AudioSampleBuffer());
        if(loopFile.existsAsFile()) loopBuffer.reset(loadSampleFromFile(loopFile));

        indx = 0;
        
    }
    
    ~Loop(){}
    
    
    void setAudioFile(String audioFilePath)
    {
        File loopFile(audioFilePath);
        loopBuffer.reset(new AudioSampleBuffer());
        if(loopFile.existsAsFile()) loopBuffer.reset(loadSampleFromFile(loopFile));
    }
    
    void setLoopDuration(float milleseconds)
    {
        loopDuration = milleseconds*SAMPLE_RATE/1000;
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

    
    void processLoop(AudioSampleBuffer& sampleBuffer, int numChannels)
    {
        for(int i = 0; i < sampleBuffer.getNumSamples(); i++)
        {

            for (int channel = 0; channel < numChannels; ++channel)
            {
                int loopChannel = numChannels%loopBuffer->getNumChannels();
                float* samples = sampleBuffer.getWritePointer(channel);
                float outSample = 0.0;
                
                if(indx < loopBuffer->getNumSamples())
                    outSample = loopBuffer->getReadPointer(loopChannel)[indx];
                
                samples[i] = signalLevel*outSample;
            }
        
        indx = (indx + 1) % loopDuration;
            
        }
    
    }
    
    float processLoopSample(int channel)
    {
        int loopChannel = channel%loopBuffer->getNumChannels();
        float outSample = 0.0;
        
        if(indx < loopBuffer->getNumSamples())
            outSample = loopBuffer->getReadPointer(loopChannel)[indx];
        
        
        indx = (indx + 1) % loopDuration;
        
        return signalLevel*outSample;
    }

    
    
    void setSignalLevel(float level)
    {
        signalLevel = level;
    }
    
    
private:
    
    std::unique_ptr<AudioSampleBuffer> loopBuffer;
    int indx;
    float signalLevel;
    
    int loopDuration;
    
};

#endif /* defined(__KissOfShame__Loop__) */
