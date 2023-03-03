#pragma once

#include <JuceHeader.h>


class PlayControlButton  :  public juce::ImageButton,
                            public juce::Button::Listener
{
public:
    PlayControlButton();
    void buttonClicked(juce::Button *) override;

    class Listener
    {
    public:
        virtual ~Listener() = default;

        virtual void startPlayButtonClicked() = 0;
        virtual void stopPlayButtonClicked() = 0;
    };

    void setListener(Listener& newListener);
    void playingStarted();
    void playingStoped();

private:
    bool playing = false;
    Listener* listener = nullptr;
    juce::Image pauseButtonImg = juce::ImageCache::getFromMemory(BinaryData::pausebutton_png, BinaryData::pausebutton_pngSize);
    juce::Image playButtonImg = juce::ImageCache::getFromMemory(BinaryData::playbutton_png, BinaryData::playbutton_pngSize);

    void updateButtonImage(const juce::Image& buttonImg);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayControlButton)
};
