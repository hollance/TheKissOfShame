#pragma once

#include <JuceHeader.h>

#define PI 3.14159265359f

// TODO: don't hardcode the sample rate
#define SAMPLE_RATE 44100

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

juce::AudioBuffer<float>* loadSampleFromFile(const juce::File& audioFile);
juce::AudioBuffer<float>* loadSampleFromMemory(const char* data, int size);
