#include "MuteButton.h"

MuteButton::MuteButton()
{
    addListener(this);
    updateButtonImage();
    updateTooltip();
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void MuteButton::buttonClicked(juce::Button *)
{
    setMuted(!isMuted());

    updateButtonImage();
    updateTooltip();
    sendChangeMessage();
}

void MuteButton::setMuted(bool newStatus)
{
    status = newStatus;
}

void MuteButton::updateButtonImage()
{
    if (isMuted())
        setButtonImage(muteButtonImg);
    else
        setButtonImage(unmuteButtonImg);
}

void MuteButton::setButtonImage(const juce::Image& buttonImg)
{
    // The idea is to have different opacity for the button images
    // when the button is in different states
    setImages(true, false, true,
                buttonImg, 1.0f, juce::Colours::transparentBlack, // Normal
                juce::Image(), 0.7f, juce::Colours::transparentBlack, // Over
                juce::Image(), 0.4f, juce::Colours::transparentBlack); // Pressed
    setBounds(getX(), getY(), buttonImg.getWidth(), buttonImg.getHeight());
}

// tooltip is a text that appears when the mouse hovers over the button
void MuteButton::updateTooltip()
{
    if (isMuted())
        setTooltip("Unmute");
    else
        setTooltip("Mute");
}
