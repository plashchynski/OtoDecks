#include <JuceHeader.h>
#include "PlayControlButton.h"

PlayControlButton::PlayControlButton() : juce::ImageButton("PlayControlButton")
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);

    addListener(this);

    // By default, the button is in the "stopped" state, showing the play button
    updateButtonImage(playButtonImg);
}

void PlayControlButton::buttonClicked(juce::Button *)
{
    setPlaying(!playing);
}

void PlayControlButton::setPlaying(bool isPlaying)
{
    if (playing == isPlaying)
        return;

    playing = isPlaying;

    if (playing)
        updateButtonImage(pauseButtonImg);
    else
        updateButtonImage(playButtonImg);

    sendChangeMessage();
}

bool PlayControlButton::isPlaying() const {
    return playing;
}

void PlayControlButton::updateButtonImage(const juce::Image& buttonImg)
{
    setImages(true, false, true,
                buttonImg, 1.0f, juce::Colours::transparentBlack, // Normal
                juce::Image(), 0.7f, juce::Colours::transparentBlack, // Over
                juce::Image(), 0.4f, juce::Colours::transparentBlack); // Pressed
}
