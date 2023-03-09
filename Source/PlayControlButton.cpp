#include <JuceHeader.h>
#include "PlayControlButton.h"

PlayControlButton::PlayControlButton() : juce::ImageButton("PlayControlButton")
{
    addListener(this);
    updateButtonImage();
    updateTooltip();
}

void PlayControlButton::buttonClicked(juce::Button *)
{
    if (status == Playing)
        setStatus(PauseRequested);
    else if (status == Paused)
        setStatus(PlayRequested);

    updateButtonImage();
    updateTooltip();
    sendChangeMessage();
}

void PlayControlButton::updateTooltip()
{
    switch(status)
    {
        case Disabled:
            setTooltip("No file loaded");
            break;
        case PauseRequested:
            setTooltip("Pausing...");
            break;
        case Playing:
            setTooltip("Pause the playback");
            break;
        case PlayRequested:
            setTooltip("Loading...");
            break;
        case Paused:
            setTooltip("Start the playback");
            break;
        default:
            break;
    }
}

void PlayControlButton::updateButtonImage()
{
    switch(status)
    {
        case Disabled:
            setDisabledButtonImage(playButtonImg);
            break;
        case PauseRequested:
            [[fallthrough]];
        case Playing:
            setButtonImage(pauseButtonImg);
            break;
        case PlayRequested:
            [[fallthrough]];
        case Paused:
            setButtonImage(playButtonImg);
            break;
        default:
            break;
    }
}

void PlayControlButton::setButtonImage(const juce::Image& buttonImg)
{
    setImages(true, false, true,
                buttonImg, 1.0f, juce::Colours::transparentBlack, // Normal
                juce::Image(), 0.7f, juce::Colours::transparentBlack, // Over
                juce::Image(), 0.4f, juce::Colours::transparentBlack); // Pressed
}

void PlayControlButton::setDisabledButtonImage(const juce::Image& buttonImg)
{
    setImages(true, false, true,
                buttonImg, 0.2f, juce::Colours::transparentBlack, // Normal
                juce::Image(), 0.2f, juce::Colours::transparentBlack, // Over
                juce::Image(), 0.2f, juce::Colours::transparentBlack); // Pressed
}

void PlayControlButton::setStatus(const PlayControlButton::Status &newStatus)
{
    status = newStatus;

    if (status == Disabled)
        setMouseCursor(juce::MouseCursor::NormalCursor);
    else
        setMouseCursor(juce::MouseCursor::PointingHandCursor);

    updateButtonImage();
    updateTooltip();
}
