#pragma once

#include <JuceHeader.h>

#define PI 3.14159265359f

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
