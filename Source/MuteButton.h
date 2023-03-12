#pragma once

#include <JuceHeader.h>

class MuteButton    :   public juce::ImageButton,
                        public juce::Button::Listener,
                        public juce::ChangeBroadcaster
{
public:
    MuteButton();

    // implement juce::Button::Listener
    void buttonClicked(juce::Button *) override;

    enum Status { Muted, Unmuted };
    
    void setStatus(const MuteButton::Status &newStatus);
    Status getStatus() const { return status; }

private:
    Status status = Unmuted;

    juce::Image muteButtonImg = juce::ImageCache::getFromMemory(BinaryData::mute_png, BinaryData::mute_pngSize);
    juce::Image unmuteButtonImg = juce::ImageCache::getFromMemory(BinaryData::unmute_png, BinaryData::unmute_pngSize);

    void updateButtonImage();
    void updateTooltip();
    void setButtonImage(const juce::Image& buttonImg);
};
