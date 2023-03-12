#pragma once

#include <JuceHeader.h>

class Player : public juce::AudioSource,
                      public juce::ChangeListener,
                      public juce::ChangeBroadcaster
{
public:
    Player(juce::AudioFormatManager& _formatManager);
    ~Player();

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

    /**
     * This is an exception that is thrown by loadURL when the file format is not supported
    */
    class UnsupportedFormatError : public std::exception {};

    /**
     * Stores the metadata (title, artist, etc.) of the loaded file
    */
    juce::StringPairArray metaData;

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
