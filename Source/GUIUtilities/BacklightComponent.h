#pragma once

#include "../shameConfig.h"

class BacklightComponent : public juce::Component
{
public:
    BacklightComponent()
    {
        setSize(960, 266);
        setInterceptsMouseClicks(false, false);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour::fromFloatRGBA(1.0f, 0.216f, 0.384f, 1.0f));
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BacklightComponent)
};
