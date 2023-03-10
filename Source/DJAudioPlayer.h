/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 24 Feb 2023 1:28:59pm
    Author:  Dzmitry Plashchynski

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource,
                      public juce::ChangeListener,
                      public juce::ChangeBroadcaster
{
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);

    void start();
    void stop();

    /**
     * @return true if the player is currently playing
     */
    bool isPlaying() const;

    // implementing juce::ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    /**
     * get the relative position of the playhead
     *
     * @return a value between 0 and 1, where 0 is the beginning of the track and 1 is the end
     */
    double getPositionRelative() const;

    /**
     * get the absolute position of the playhead
     * 
     * @return the position in seconds
    */
    double getPositionAbsolute() const;

    /**
     * get the length of the track
     * 
     * @return the length in seconds
    */
    double getLengthInSeconds() const;

    /**
     * Set the relative position of the playhead
     *
     * @param pos a value between 0 and 1, where 0 is the beginning of the track and 1 is the end
    */
    void setPositionRelative(double pos);

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
