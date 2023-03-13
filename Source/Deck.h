/**
 * This class represents a set of controls for one playing unit.
*/
#pragma once

#include <JuceHeader.h>
#include "Player.h"
#include "WaveformSlider.h"
#include "PlayControlButton.h"
#include "Fader.h"
#include "ControlButton.h"
#include "MuteButton.h"

class Deck   :  public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::DragAndDropTarget,
                public juce::Timer,
                public juce::ChangeListener,
                public juce::ChangeBroadcaster
{
public:
    /**
     * Constructor.
     *
     * @param _formatManager A reference to a global format manager.
     * @param cacheToUse A reference to a global thumbnail cache.
     * */
    Deck(juce::AudioFormatManager& _formatManager,
            juce::AudioThumbnailCache& cacheToUse);
    ~Deck();

    void paint(juce::Graphics&) override;
    void resized() override;

     /** implement juce::Button::Listener */
    void buttonClicked(juce::Button *) override;

    /** implement juce::Slider::Listener */
    void sliderValueChanged(juce::Slider *slider) override;

    /** implement juce::FileDragAndDropTarget */
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray &files, int x, int y) override;

    /** implement juce::DragAndDropTarget */
    bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;
    void itemDropped(const SourceDetails& dragSourceDetails) override;

    /** implement juce::Timer */
    void timerCallback() override;

    /** implement juce::ChangeListener */
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    /**
     * Load a file into the deck
     *
     * @param file the file to load
    */
    void loadFile(juce::File file);

    /**
     * This is the playing backend for the deck.
     * It is public because it is used in the main component to control the playback.
    */
    Player player;

    /**
     * This is a flag to indicate that the deck should be removed.
     * It is used by the main component to remove the deck from the screen.
     */
    bool toBeRemoved = false;

private:
    // Button to remove the deck from the screen
    ControlButton removeButton{
        juce::ImageCache::getFromMemory(BinaryData::remove_png, BinaryData::remove_pngSize),
        "Remove the deck"
    };

    // Button to load a file into the deck
    ControlButton loadButton{
        juce::ImageCache::getFromMemory(BinaryData::ejectbutton_png, BinaryData::ejectbutton_pngSize),
        "Load a file to the deck"
    };

    // Button to start/pause the playback
    PlayControlButton playControlButton;

    // Button to mute the deck
    MuteButton muteButton;

    // Faders (sliders) to control the volume and playback speed
    Fader volumeFader{"Volume", Fader::Type::Vertical, "percent", 0.0, 1.0, 1.0};
    Fader speedFader{"Speed", Fader::Type::Vertical, "percent", 0.0, 3.0, 1.0};

    // Labels to display meta-infomation about the loaded track
    juce::Label durationLabel;
    juce::Label titleLabel;

    // A waveform display that combined with a position slider
    WaveformSlider waveformSlider;

    // A reference to a global format manager
    juce::AudioFormatManager& formatManager;

    // an instance represents a dialog to select a file
    juce::FileChooser fileChooser{"Select a file...", juce::File(), formatManager.getWildcardForAllFormats()};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Deck)
};
