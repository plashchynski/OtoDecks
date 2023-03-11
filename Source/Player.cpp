#include "Player.h"

Player::Player(juce::AudioFormatManager& _formatManager)
: formatManager(_formatManager)
{
    transportSource.addChangeListener(this);
}

Player::~Player()
{

}

void Player::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void Player::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);

}

void Player::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void Player::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inAddress)));
    if (reader != nullptr) // good file!
    {
        metaData = reader->metadataValues;
        std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource (reader, true));
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset (newSource.release());
    }
    else
        throw UnsupportedFormatError{};
}

void Player::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "Player::setGain gain should be between 0 and 1" << std::endl;
    }
    else
    {
        transportSource.setGain(gain);
    }
}

void Player::setSpeed(double ratio)
{
  if (ratio < 0 || ratio > 100.0)
    {
        std::cout << "Player::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void Player::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void Player::setPositionRelative(double pos)
{
     if (pos < 0 || pos > 1.0)
    {
        std::cout << "Player::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void Player::start()
{
    transportSource.start();
}

void Player::stop()
{
    transportSource.stop();
}

double Player::getPositionRelative() const
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double Player::getPositionAbsolute() const
{
    return transportSource.getCurrentPosition();
}

double Player::getLengthInSeconds() const
{
    return transportSource.getLengthInSeconds();
}

bool Player::isPlaying() const
{
    return transportSource.isPlaying();
}

// implementing juce::ChangeListener
void Player::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    /**
     * If the playback status (playing/stopped) has changed, we need to notify the listeners,
     * to propagate the change to the listeners to update the GUI.
     * */ 
    if (source == &transportSource)
    {
        sendChangeMessage();
    }
}
