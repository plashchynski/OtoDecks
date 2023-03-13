/**
 * This class represents a button with a custom image and tooltip text.
 * It also sets the appropriate mouse pointer style when hovering over the button.
 * It's used to reduce the boilerplate code in components.
 */

#pragma once

#include <JuceHeader.h>

class ControlButton : public juce::ImageButton
{
public:
    /**
      * Constructor.
      *
      * @param image The image to be displayed on the button.
      * @param tooltipText The text to be displayed when hovering over the button.
      */
    ControlButton(juce::Image image, juce::String tooltipText);
};
