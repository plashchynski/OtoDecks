#pragma once

#include <JuceHeader.h>

class Fader :   public juce::Component,
                public juce::Slider::Listener,
                public juce::ChangeBroadcaster
{
public:
    enum Type { Vertical, Horizontal };

    Fader(juce::String name, Type type, juce::String unit, double minValue, double maxValue, double defaultValue);

    void paint(juce::Graphics&) override;
    void resized() override;

    /** implement juce::Slider::Listener */
    void sliderValueChanged(juce::Slider *slider) override;

    double getValue() const;

private:
    Type type;

    double minValue;
    double maxValue;
    double defaultValue;
    juce::String unit;

    juce::Label label;
    juce::Label valueLabel;
    juce::Slider slider{juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::NoTextBox};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Fader)
};
