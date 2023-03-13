/**
 * This class represents a slider that shows the waveform of the audio file
 * and allows the user to drag the playhead to a specific position
*/

#pragma once

#include <JuceHeader.h>

class WaveformSlider:   public juce::Slider,
                        public juce::ChangeListener
{
public:
    /**
     * Constructor
     *
     * @param formatManagerToUse a global audio format manager
     * @param cacheToUse a global audio thumbnail cache
    */
    WaveformSlider(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);
    ~WaveformSlider();

    // Implements juce::ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    /**
     * Load a new audio file from a URL to display the waveform
     *
     * @param audioURL the URL of the audio file
    */
    void loadURL(juce::URL audioURL);

private:
    // the instance to display the waveform
    juce::AudioThumbnail audioThumb;

    bool fileLoaded;

    // the LookAndFeel interface to customize the slider appearance
    class LookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        LookAndFeel(juce::AudioThumbnail& audioThumb , bool& fileLoaded) : audioThumb(audioThumb), fileLoaded(fileLoaded) {}

        // this method is used to draw the waveform on the background of the slider
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
