/*
  ==============================================================================

    PlayControlButton.cpp
    Created: 3 Mar 2023 3:44:03pm
    Author:  Dzmitry Plashchynski

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlayControlButton.h"

PlayControlButton::PlayControlButton() : juce::ImageButton("PlayControlButton")
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);

    addListener(this);

    // By default, the button is in the "stopped" state
    updateButtonImage(playButtonImg);
}

void PlayControlButton::buttonClicked(juce::Button *)
{
    if (listener != nullptr)
    {
        if (playing)
            listener->pauseButtonClicked();
        else
            listener->playButtonClicked();
    }
}

void PlayControlButton::setListener(Listener* newListener)
{
    listener = newListener;
}

void PlayControlButton::playingStarted()
{
    playing = true;
    updateButtonImage(pauseButtonImg);
}

void PlayControlButton::playingStopped()
{
    playing = false;
    updateButtonImage(playButtonImg);
}

void PlayControlButton::updateButtonImage(const juce::Image& buttonImg)
{
    setImages(true, false, true,
                buttonImg, 1.0f, juce::Colours::transparentBlack, // Normal
                juce::Image(), 0.7f, juce::Colours::transparentBlack, // Over
                juce::Image(), 0.4f, juce::Colours::transparentBlack); // Pressed
}
