#include <JuceHeader.h>
#include "PlayControlButton.h"

PlayControlButton::PlayControlButton() : juce::ImageButton("PlayControlButton")
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);

    addListener(this);
    updateButtonImage();
}

void PlayControlButton::buttonClicked(juce::Button *)
{
    if (status == Playing)
        status = PauseRequested;
    else if (status == Paused)
        status = PlayRequested;

    updateButtonImage();
    sendChangeMessage();
}

void PlayControlButton::setPlaying(bool isPlaying)
{
    if (isPlaying)
        status = Playing;
    else
        status = Paused;

    updateButtonImage();
}

void PlayControlButton::updateButtonImage()
{
    switch(status)
    {
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
