#pragma once

#include <JuceHeader.h>

class WaveformSlider  :  public juce::Slider,
                          public juce::ChangeListener
{
public:
    WaveformSlider(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);

    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    void loadURL(juce::URL audioURL);

    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);

private:
    juce::AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    class LookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        LookAndFeel(juce::AudioThumbnail& audioThumb , bool& fileLoaded) : audioThumb(audioThumb), fileLoaded(fileLoaded) {}
        void drawLinearSlider (juce::Graphics&, int x, int y, int width, int height,
                            float sliderPos, float minSliderPos, float maxSliderPos,
                            const Slider::SliderStyle, Slider&) override;
    private:
        juce::AudioThumbnail& audioThumb;
        bool& fileLoaded;
    };

    LookAndFeel lookAndFeel{audioThumb, fileLoaded};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformSlider)
};
