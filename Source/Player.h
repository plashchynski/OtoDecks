/**
 * This class represents a playing backend used by Deck class.
*/

#pragma once

#include <JuceHeader.h>

class Player : public juce::AudioSource,
                      public juce::ChangeListener,
                      public juce::ChangeBroadcaster
{
public:
    /**
     * Constructor
     *
     * @param _formatManager a global audio format manager
    */
    Player(juce::AudioFormatManager& _formatManager);
    ~Player();

    // implementing juce::AudioSource
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    /**
     * Load a new audio file from a URL
     *
     * @param audioURL the URL of the audio file
    */
    void loadURL(juce::URL audioURL);

    /**
     * Set the gain (volume) of the player
     *
     * @param gain the gain value from 0 to 1
    */
    void setGain(double gain);

    /**
     * Set the speed of the player
     *
     * @param ratio the speed ratio from 0 to 1
    */
    void setSpeed(double ratio);

    /**
     * get the relative position of the playhead
     *
     * @return a value between 0 and 1, where 0 is the beginning of the track and 1 is the end
     */
    double getPositionRelative() const;

    /**
     * Set the relative position of the playhead
     *
     * @param pos a value between 0 and 1, where 0 is the beginning of the track and 1 is the end
    */
    void setPositionRelative(double pos);

    /**
     * get the absolute position of the playhead
     *
     * @return the position in seconds
    */
    double getPositionAbsolute() const;

    /**
     * Set the position of the playhead
     *
     * @param posInSecs the position in seconds
    */
    void setPosition(double posInSecs);

    /**
     * get the length of the track
     *
     * @return the length in seconds
    */
    double getLengthInSeconds() const;

    /**
     * Start the playback
    */
    void start();

    /**
     * Stop the playback
    */
    void stop();

    /**
     * @return true if the player is currently playing
     */
    bool isPlaying() const;

    // implementing juce::ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    /**
     * This is an exception that is thrown by loadURL when the file format is not supported
    */
    class UnsupportedFormatError : public std::exception {};

    /**
     * Stores the metadata (title, artist, etc.) of the loaded file
    */
    juce::StringPairArray metaData;

private:
    // reference to the global audio format manager
    juce::AudioFormatManager& formatManager;

    // internals of the playing backend
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
