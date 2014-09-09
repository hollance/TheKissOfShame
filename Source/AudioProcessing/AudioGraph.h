//
//  AudioGraph.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/9/14.
//
//

#ifndef __KissOfShame__AudioGraph__
#define __KissOfShame__AudioGraph__

#include <iostream>

#include "../../JuceLibraryCode/JuceHeader.h"

#include "Hiss.h"
#include "Shame.h"
#include "InputSaturation.h"



class AudioGraph
{
public:
    
    
    AudioGraph()
    {
        hiss = new Hiss();
        shame = new Shame();
        inSaturation = new InputSaturation(0.1, 2.0);
    }
    
    ~AudioGraph(){}
    
    
    float processGraph(float sample)
    {
        return inSaturation->processInputSaturation(sample);
    }
    
private:
    
    ScopedPointer<Hiss> hiss;
    ScopedPointer<Shame> shame;
    ScopedPointer<InputSaturation> inSaturation;
    
    
};


#endif /* defined(__KissOfShame__AudioGraph__) */
