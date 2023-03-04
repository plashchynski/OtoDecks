#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformSlider.h"
#include "PlayControlButton.h"

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
                    public juce::ChangeListener
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

private:
    PlayControlButton playControlButton;

    juce::TextButton loadButton{"LOAD"};

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    juce::FileChooser fChooser{"Select a file..."};

    WaveformSlider waveformSlider;

    juce::AudioFormatManager& formatManager;

    DJAudioPlayer* player;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
