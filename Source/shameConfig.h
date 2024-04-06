#pragma once

#include <JuceHeader.h>

#define PI 3.14159265359
#define SAMPLE_RATE 44100

//TODO: horrible hack!
#if JUCE_MSVC
#define GUI_PATH juce::String("C:\\Program Files\\Common Files\\VST3\\KissOfShame\\GUI_Resources\\")
#define AUDIO_PATH juce::String("C:\\Program Files\\Common Files\\VST3\\KissOfShame\\Audio_Resources\\")
#else
#define GUI_PATH juce::String("/Users/Shared/KissOfShame/GUI_Resources/")
#define AUDIO_PATH juce::String("/Users/Shared/KissOfShame/Audio_Resources/")
#endif

enum EShameEnvironments
{
    eEnvironmentOff,
    eEnvironmentEnvironment,
    eEnvironmentStudioCloset,
    eEnvironmentHumidCellar,
    eEnvironmentHotLocker,
    eEnvironmentHurricaneSandy,
};

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
    eBlendLevel,
    eFlangeDepth,

    eBypass,
    eInputDrive,
    eOutputLevel,
};

juce::AudioBuffer<float>* loadSampleFromFile(const juce::File& audioFile);
juce::AudioBuffer<float>* loadSampleFromMemory(const char* data, int size);
