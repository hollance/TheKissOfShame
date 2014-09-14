//
//  Hiss.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/9/14.
//
//

#ifndef __KissOfShame__Hiss__
#define __KissOfShame__Hiss__

#include <iostream>

#include "../shameConfig.h"

class Hiss
{
public:
    Hiss()
    {
        File hissFile("/Users/brianhansen/Documents/Brian/Work/1_KOS/kissofshame/Audio_Resources/Hiss.wav");
        hissBuffer = new AudioSampleBuffer();
        if(hissFile.existsAsFile()) hissBuffer = loadSampleFromFile(hissFile);
        
        hissLevel = 0.0;
        
        indx1 = 0;
        indx2 = hissBuffer->getNumSamples()/2;
        
//        crossfade = false;
    }
    
    ~Hiss(){}
    
    
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

    
    AudioSampleBuffer *loadSampleFromFile(const File& audioFile) {
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

    
    void processHiss(AudioSampleBuffer& sampleBuffer, int numChannels)
    {
        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* samples = sampleBuffer.getWritePointer(channel);
            
            for(int i = 0; i < sampleBuffer.getNumSamples(); i++)
            {
                //
//                if(indx1 > hissBuffer->getNumSamples()*0.5) crossfade = true;
                
                
                //Calculate ramp value for crossfading
                if(indx1 < 0.5*hissBuffer->getNumSamples())
                    rampValue = (float)indx1 / (0.5*(float)hissBuffer->getNumSamples());
                else
                    rampValue = (float)(hissBuffer->getNumSamples() - indx1) / (0.5*(float)hissBuffer->getNumSamples());
                
//NOTE: not working...
//                if(crossfade)
//                    hissSample = rampValue*hissBuffer->getReadPointer(channel)[indx1] + (1-rampValue)*hissBuffer->getReadPointer(channel)[indx2];
//                else
//                    hissSample = hissBuffer->getReadPointer(channel)[indx1];
                
                
                hissSample = rampValue*hissBuffer->getReadPointer(channel)[indx1] + (1-rampValue)*hissBuffer->getReadPointer(channel)[indx2];
                
            
                samples[i] = (1-hissLevel)*samples[i] + hissLevel*hissSample;
                
                
                indx1 = (indx1 + 1) % hissBuffer->getNumSamples();
                //if(crossfade) indx2 = (indx2 + 1) % hissBuffer->getNumSamples();
                indx2 = (indx2 + 1) % hissBuffer->getNumSamples();
            }
        }
    }
    
    
    void setHissLevel(float level) {hissLevel = level;}
    
private:
    
    ScopedPointer<AudioSampleBuffer> hissBuffer;
    int indx1;
    int indx2;
    float hissSample;
    float hissLevel;
    
    
    float rampValue;
    
//    bool crossfade;
    
};

#endif /* defined(__KissOfShame__Hiss__) */
