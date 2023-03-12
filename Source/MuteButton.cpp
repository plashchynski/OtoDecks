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
    if (getStatus() == Unmuted)
        setStatus(Muted);
    else
        setStatus(Unmuted);
    
    updateButtonImage();
    updateTooltip();
    sendChangeMessage();
}

void MuteButton::setStatus(const MuteButton::Status &newStatus)
{
    status = newStatus;
}

void MuteButton::updateButtonImage()
{
    switch(status)
    {
        case Muted:
            setButtonImage(muteButtonImg);
            break;
        case Unmuted:
            setButtonImage(unmuteButtonImg);
            break;
        default:
            break;
    }
}

void MuteButton::setButtonImage(const juce::Image& buttonImg)
{
    setImages(true, false, true,
                buttonImg, 1.0f, juce::Colours::transparentBlack, // Normal
                juce::Image(), 0.7f, juce::Colours::transparentBlack, // Over
                juce::Image(), 0.4f, juce::Colours::transparentBlack); // Pressed
    setBounds(getX(), getY(), buttonImg.getWidth()+15, buttonImg.getHeight()+15);
}

void MuteButton::updateTooltip()
{
    switch(status)
    {
        case Muted:
            setTooltip("Unmute");
            break;
        case Unmuted:
            setTooltip("Mute");
            break;
        default:
            break;
    }
}
