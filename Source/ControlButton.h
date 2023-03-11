#pragma once

#include <JuceHeader.h>

class ControlButton : public juce::ImageButton
{
public:
    ControlButton(juce::Image image, juce::String tooltipText);
};
