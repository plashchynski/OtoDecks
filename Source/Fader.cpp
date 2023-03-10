#include "Fader.h"

Fader::Fader(juce::String name, juce::String _unit, double _minValue, double _maxValue, double _defaultValue) :
    unit(_unit), minValue(_minValue), maxValue(_maxValue), defaultValue(_defaultValue)
{
    addAndMakeVisible(label);
    addAndMakeVisible(valueLabel);
    addAndMakeVisible(slider);

    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    valueLabel.setJustificationType(juce::Justification::centred);

    slider.addListener(this);
    slider.setRange(minValue, maxValue);
    slider.setValue(defaultValue);

    slider.setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
}

Fader::~Fader()
{
}

void Fader::paint(juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void Fader::resized()
{
    auto area = getLocalBounds();
    auto labelArea = area.removeFromTop(20);
    label.setBounds(labelArea);
    auto valueArea = area.removeFromTop(20);
    valueLabel.setBounds(valueArea);
    slider.setBounds(area);
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
