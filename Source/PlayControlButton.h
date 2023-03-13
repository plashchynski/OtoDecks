/**
 * This class represents a play control button — a special button that used to
 * play or pause a deck
*/
#pragma once

#include <JuceHeader.h>


class PlayControlButton  :  public juce::ImageButton,
                            public juce::Button::Listener,
                            public juce::ChangeBroadcaster
{
public:
    // Constructor
    PlayControlButton();

    // implement juce::Button::Listener
    void buttonClicked(juce::Button *) override;

    // There a whole bunch of statuses that a play control button can have
    enum Status { Disabled, Playing, Paused, PlayRequested, PauseRequested };

    /**
     * Update the status of the button.
     *
     * @param newStatus The new status of the button.
    */
    void setStatus(const PlayControlButton::Status &newStatus);

    /**
     * @return The current status of the button.
    */
    Status getStatus() const { return status; }

private:
    // Stores the current status of the button
    Status status = Disabled;

    // Images used for different statuses
    juce::Image pauseButtonImg = juce::ImageCache::getFromMemory(BinaryData::pausebutton_png, BinaryData::pausebutton_pngSize);
    juce::Image playButtonImg = juce::ImageCache::getFromMemory(BinaryData::playbutton_png, BinaryData::playbutton_pngSize);

    // Update the button image and tooltip
    void updateButtonImage();
    void updateTooltip();
    void setButtonImage(const juce::Image& buttonImg);
    void setDisabledButtonImage(const juce::Image& buttonImg);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayControlButton)
};
