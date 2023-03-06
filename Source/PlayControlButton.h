#pragma once

#include <JuceHeader.h>


class PlayControlButton  :  public juce::ImageButton,
                            public juce::Button::Listener,
                            public juce::ChangeBroadcaster
{
public:
    PlayControlButton();
    void buttonClicked(juce::Button *) override;
    void setPlaying(bool isPlaying);
    bool isPlaying() const;
    enum Status { Playing, Paused, PlayRequested, PauseRequested };
    Status getStatus() const { return status; }

private:
    Status status = Paused;

    juce::Image pauseButtonImg = juce::ImageCache::getFromMemory(BinaryData::pausebutton_png, BinaryData::pausebutton_pngSize);
    juce::Image playButtonImg = juce::ImageCache::getFromMemory(BinaryData::playbutton_png, BinaryData::playbutton_pngSize);

    void updateButtonImage();
    void setButtonImage(const juce::Image& buttonImg);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayControlButton)
};
