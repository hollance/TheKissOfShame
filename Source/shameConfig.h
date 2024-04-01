#pragma once

#include <JuceHeader.h>

#define BUFFER_SIZE (44100)
#define PI 3.14159265359
#define SAMPLE_RATE 44100

//TODO: horrible hack!
#if JUCE_MSVC
#define GUI_PATH String("C:\\Program Files\\Common Files\\VST3\\KissOfShame\\GUI_Resources\\")
#define AUDIO_PATH String("C:\\Program Files\\Common Files\\VST3\\KissOfShame\\Audio_Resources\\")
#else
#define GUI_PATH String("/Users/Shared/KissOfShame/GUI_Resources/")
#define AUDIO_PATH String("/Users/Shared/KissOfShame/Audio_Resources/")
#endif

enum EShameEnvironments
{
    eEnvironmentOff,
    eEnvironmentEnvironment,
    eEnvironmentStudioCloset,
    eEnvironmentHumidCellar,
    eEnvironmentHotLocker,
    eEnvironmentHurricaneSandy,
    eEnvironmentTotalEnvironments,
};

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

    eHurricaneSandyGlobalLevel,
    eHissLevel,
    eBlendLevel,
    eFlangeDepth,

    eBypass,
    eInputDrive,
    eOutputLevel,
};
