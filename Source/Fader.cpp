#include "Fader.h"

Fader::Fader(juce::String name, Type _type, juce::String _unit, double _minValue, double _maxValue, double _defaultValue) :
    unit(_unit), minValue(_minValue), maxValue(_maxValue), defaultValue(_defaultValue), type(_type)
{
    addAndMakeVisible(label);
    addAndMakeVisible(valueLabel);
    addAndMakeVisible(slider);

    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    valueLabel.setJustificationType(juce::Justification::centred);

    if (type == Type::Vertical)
    {
        slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        slider.setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
    }
    else
    {
        slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        slider.setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
    }

    slider.addListener(this);
    slider.setRange(minValue, maxValue);
    slider.setValue(defaultValue);
}

void Fader::paint(juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void Fader::resized()
{
    if (type == Type::Vertical)
    {
        /**
         * +--------+
         * | label  |
         * +--------+
         * | value  |
         * +--------+
         * | slider |
         * +--------+
        */

        auto area = getLocalBounds();
        auto labelArea = area.removeFromTop(20);
        label.setBounds(labelArea);
        auto valueArea = area.removeFromTop(20);
        valueLabel.setBounds(valueArea);
        slider.setBounds(area);
    }
    else if (type == Type::Horizontal)
    {
        /**
         * +--------+--------+
         * | label  | value  |
         * +--------+--------+
         * | slider          |
         * +--------+--------+
        */

        using Track = juce::Grid::TrackInfo;
        using Fr = juce::Grid::Fr;

        juce::Grid grid;
        grid.templateRows = { Track(Fr(1)), Track(Fr(2)) };
        grid.templateColumns = { Track(Fr(2)), Track(Fr(1)) };

        grid.items.addArray({
            juce::GridItem(label),
            juce::GridItem(valueLabel),
            juce::GridItem(slider).withArea(2, 1, 2, 3)
        });

        grid.performLayout(getLocalBounds());
    }
}

void Fader::sliderValueChanged(juce::Slider* slider)
{
    if (unit == "percent")
    {
        valueLabel.setText(juce::String::formatted("%d%%", (int)(slider->getValue() * 100)), juce::dontSendNotification);
    }
    else
    {
        valueLabel.setText(juce::String::formatted("%d " + unit, (int)(slider->getValue())), juce::dontSendNotification);
    }

    sendChangeMessage();
}

double Fader::getValue() const
{
    return slider.getValue();
}

void Fader::setValue(double value)
{
    slider.setValue(value);
}
