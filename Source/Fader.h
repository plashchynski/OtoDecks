/**
 * This class represents a fader — a vertical or horizontal slider with a
 * name and value indication.
*/

#pragma once

#include <JuceHeader.h>

class Fader :   public juce::Component,
                public juce::Slider::Listener,
                public juce::ChangeBroadcaster
{
public:
    // Type of placement of the fader.
    enum Type { Vertical, Horizontal };

    /**
     * Constructor.
     *
     * @param name The name of the fader displayed on the top of the fader.
     * @param type The type of the fader (vertical or horizontal).
     * @param unit The unit of the fader (percent, dB, etc.)
     * @param minValue The minimum value of the fader. It should be from 0 to 1 for a percent fader.
     * @param maxValue The maximum value of the fader.
     * @param defaultValue The default value of the fader.
     *
    */
    Fader(juce::String name, Type type, juce::String unit, double minValue, double maxValue, double defaultValue);
    ~Fader();

    // juce::Component overrides
    void paint(juce::Graphics&) override;
    void resized() override;

    /** implement juce::Slider::Listener */
    void sliderValueChanged(juce::Slider *slider) override;

    /**
     * Set the value of the fader.
     *
     * @param value The value to set.
     *
    */
    void setValue(double value);

    /**
     * Get the value of the fader.
     *
     * @return The value of the fader.
    */
    double getValue() const;

private:
    // Fader configuration saved in the constructor:
    Type type;
    double minValue;
    double maxValue;
    double defaultValue;
    juce::String unit;

    juce::Label label;
    juce::Label valueLabel;
    juce::Slider slider{juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::NoTextBox};

    // Custom look and feel for the fader.
    class LookAndFeel : public juce::LookAndFeel_V3
    {
    public:
        // change the look of the fader's thumb (handle)
        void drawLinearSliderThumb(juce::Graphics&, int x, int y, int width, int height,
                                float sliderPos, float minSliderPos, float maxSliderPos,
                                const juce::Slider::SliderStyle, juce::Slider&) override;
    };
    LookAndFeel lookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Fader)
};
