/*
  ==============================================================================

    DeckGUI.h
    Created: 24 Feb 2023 1:29:23pm
    Author:  Dzmitry Plashchynski

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager & 	formatManagerToUse,
            juce::AudioThumbnailCache & 	cacheToUse );
    ~DeckGUI();

    void paint (juce::Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (juce::Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (juce::Slider *slider) override;

    bool isInterestedInFileDrag (const juce::StringArray &files) override;
    void filesDropped (const juce::StringArray &files, int x, int y) override;

    void timerCallback() override;

private:

    juce::TextButton playButton{"PLAY"};
    juce::TextButton stopButton{"STOP"};
    juce::TextButton loadButton{"LOAD"};

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    juce::FileChooser fChooser{"Select a file..."};

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
