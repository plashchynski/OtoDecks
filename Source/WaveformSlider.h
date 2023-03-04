#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformSlider  :  public juce::Component,
                          public juce::ChangeListener
{
public:
    WaveformSlider(juce::AudioFormatManager & 	formatManagerToUse,
                    juce::AudioThumbnailCache & 	cacheToUse);
    ~WaveformSlider() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback (juce::ChangeBroadcaster *source) override;

    void loadURL(juce::URL audioURL);

    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);

private:
    juce::AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformSlider)
};
