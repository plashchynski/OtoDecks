#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformSlider.h"
#include "PlayControlButton.h"
#include "Fader.h"
#include "ControlButton.h"

//==============================================================================
/*
*/
class DeckGUI   :   public juce::Component,
                    public juce::Button::Listener,
                    public juce::Slider::Listener,
                    public juce::FileDragAndDropTarget,
                    public juce::DragAndDropTarget,
                    public juce::TextEditor::Listener,
                    public juce::Timer,
                    public juce::ChangeListener,
                    public juce::ChangeBroadcaster
{
public:
    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager& _formatManager,
            juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI();

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
     * @param file the file to load
    */
    void loadFile(juce::File file);

    bool toBeRemoved = false;

private:
    PlayControlButton playControlButton;

    ControlButton removeButton{
        juce::ImageCache::getFromMemory(BinaryData::remove_png, BinaryData::remove_pngSize),
        "Remove the deck"
    };

    ControlButton loadButton{
        juce::ImageCache::getFromMemory(BinaryData::ejectbutton_png, BinaryData::ejectbutton_pngSize),
        "Load a file to the deck"
    };

    Fader volumeFader{"Volume", Fader::Type::Vertical, "percent", 0.0, 1.0, 1.0};
    Fader speedFader{"Speed", Fader::Type::Vertical, "percent", 0.0, 3.0, 1.0};

    juce::Label titleLabel;
    juce::Label durationLabel;

    WaveformSlider waveformSlider;

    juce::AudioFormatManager& formatManager;
    juce::FileChooser fileChooser{"Select a file...", juce::File(), formatManager.getWildcardForAllFormats()};

    DJAudioPlayer* player;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
