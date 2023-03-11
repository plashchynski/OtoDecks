#include <JuceHeader.h>

#include "ControlButton.h"

ControlButton::ControlButton(juce::Image image, juce::String tooltipText)
{
    setImages(true, false, true,
        image, 1.0f, juce::Colours::transparentBlack,
        image, 0.7f, juce::Colours::transparentBlack,
        image, 0.4f, juce::Colours::transparentBlack);

    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    setTooltip(tooltipText);
}
