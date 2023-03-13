#include <JuceHeader.h>

#include "ControlButton.h"

/**
 * Constructor.
 *
 * @param image The image to be displayed on the button.
 * @param tooltipText The text to be displayed when hovering over the button.
 */
ControlButton::ControlButton(juce::Image image, juce::String tooltipText)
{
    // The idea is to have different opacity levels for the image when the button is
    // normal, hovered over, or pressed.
    setImages(true, false, true,
        image, 1.0f, juce::Colours::transparentBlack,
        image, 0.7f, juce::Colours::transparentBlack,
        image, 0.4f, juce::Colours::transparentBlack);

    // Cursor style when hovering over the button.
    setMouseCursor(juce::MouseCursor::PointingHandCursor);

    // A text to be displayed when hovering over the button.
    setTooltip(tooltipText);
}
