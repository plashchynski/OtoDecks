#include <JuceHeader.h>
#include "MixerControlPanel.h"

MixerControlPanel::MixerControlPanel()
{
    addAndMakeVisible(masterVolumeFader);

    masterVolumeFader.addChangeListener(this);

    masterVolumeFader.setValue(juce::SystemAudioVolume::getGain());
}

void MixerControlPanel::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("MixerControlPanel", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void MixerControlPanel::resized()
{
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;

    fb.items.add(juce::FlexItem(masterVolumeFader).withWidth(150.0f).withFlex(0, 0));

    fb.performLayout(getLocalBounds().toFloat());
}

void MixerControlPanel::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &masterVolumeFader)
    {
        juce::SystemAudioVolume::setGain(masterVolumeFader.getValue());
    }
}
