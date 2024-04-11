#pragma once

#include <JuceHeader.h>

#define PI 3.14159265359f
#define SAMPLE_RATE 44100

//TODO: horrible hack!
#if JUCE_MSVC
#define GUI_PATH juce::String("C:\\Program Files\\Common Files\\VST3\\KissOfShame\\GUI_Resources\\")
#else
#define GUI_PATH juce::String("/Users/Shared/KissOfShame/GUI_Resources/")
#endif

enum EShameEnvironments
{
    eEnvironmentEnvironment,
    eEnvironmentOff,
    eEnvironmentStudioCloset,
    eEnvironmentHumidCellar,
    eEnvironmentHotLocker,
    eEnvironmentHurricaneSandy,
    eNumEnvironments,
};

//TODO: remove this
enum AUParameter
{
//TODO: these parameters are not used anywhere
//    eSaturationDrive,
//    eSaturationOuput,
//    eSaturationRate,
//    eSaturationThresh,
//    eSaturationGlobalLevel,

    eShameGlobalLevel,

//TODO: these parameters are not used anywhere
//    eShameFreq,
//    eShameDepth,

    eHurricaneSandyGlobalLevel,
    eHissLevel,
//    eBlendLevel,
//    eFlangeDepth,

//    eBypass,
    eInputDrive,
    eOutputLevel,
};

juce::AudioBuffer<float>* loadSampleFromFile(const juce::File& audioFile);
juce::AudioBuffer<float>* loadSampleFromMemory(const char* data, int size);
