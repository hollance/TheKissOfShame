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


#define CURRENT_APPLICATION_PATH (File::getSpecialLocation(File::currentApplicationFile).getFullPathName())
#define GUI_PATH CURRENT_APPLICATION_PATH + "/Contents/Resources/GUI_Resources/"



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
