//
//  shameConfig.h
//  KissOfShame
//
//  Created by Brian Hansen on 9/11/14.
//
//

#ifndef KissOfShame_shameConfig_h
#define KissOfShame_shameConfig_h

#include <iostream>
#include <math.h>

#include "../JuceLibraryCode/JuceHeader.h"


#define BUFFER_SIZE (44100)
#define PI 3.14159265359
#define SAMPLE_RATE 44100



#define GUI_PATH String("/Users/brianhansen/Documents/Brian/Work/1_KOS/kissofshame/GUI_Resources/")
//#define GUI_PATH String("/Users/Shared/KissOfShame/GUI_Resources/")

#define AUDIO_PATH String("/Users/brianhansen/Documents/Brian/Work/1_KOS/kissofshame/Audio_Resources/")
//#define AUDIO_PATH String("/Users/Shared/KissOfShame/Audio_Resources/")



enum AUParameter
{
    eSaturationDrive,
    eSaturationOuput,
    eSaturationRate,
    eSaturationThresh,
    eSaturationGlobalLevel,
    
    eShameGlobalLevel,
    eShameFreq,
    eShameDepth,
    
    eHissLevel,
    
    eBlendLevel,
    
    eBypass,
    
    eOutputLevel
};



#endif
