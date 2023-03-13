/**
 * This class represents a mute button — a special button that can be used to
 * silent a deck or all decks
*/
#pragma once

#include <JuceHeader.h>

class MuteButton    :   public juce::ImageButton,
                        public juce::Button::Listener,
                        public juce::ChangeBroadcaster
{
public:
    // Constructor
    MuteButton();

    // implement juce::Button::Listener
    void buttonClicked(juce::Button *) override;

    /**
     * Update the status muted or unmuted.
    */
    void setMuted(bool newStatus);

    /**
     * @return true if it's muted or false otherwise
    */
    bool isMuted() const { return status; }

private:
    // muted?
    bool status = false;

    // images used for muted/unmuted status
    juce::Image muteButtonImg = juce::ImageCache::getFromMemory(BinaryData::mute_png, BinaryData::mute_pngSize);
    juce::Image unmuteButtonImg = juce::ImageCache::getFromMemory(BinaryData::unmute_png, BinaryData::unmute_pngSize);

    // update the button image and tooltip
    void updateButtonImage();
    void updateTooltip();
    void setButtonImage(const juce::Image& buttonImg);
};
