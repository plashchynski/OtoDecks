#pragma once

#include <JuceHeader.h>
#include "Fader.h"

class MixerControlPanel  :  public juce::Component,
                            public juce::ChangeListener
{
public:
    MixerControlPanel();

    void paint (juce::Graphics&) override;
    void resized() override;

    /** implement juce::ChangeListener */
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

private:
    Fader masterVolumeFader{"Master Volume", Fader::Type::Horizontal, "percent", 0, 1, 1};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MixerControlPanel)
};
