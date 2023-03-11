#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "LibraryComponent.h"
#include "Fader.h"

class MainComponent :   public juce::AudioAppComponent,
                        public juce::DragAndDropContainer,
                        public juce::ChangeListener,
                        public juce::Button::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

    void addDeck();

    // Implements juce::ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    // Implements juce::Button::Listener
    void buttonClicked(juce::Button *button) override;

private:
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{100};

    std::vector<DJAudioPlayer*> players;
    std::vector<DeckGUI*> decks;

    juce::MixerAudioSource mixerSource;

    juce::ImageButton addDeckButton;
    Fader masterVolumeFader{"Master Volume", Fader::Type::Horizontal, "percent", 0, 1, 1};

    LibraryComponent libraryComponent{formatManager};

    juce::TooltipWindow tooltipWindow{this, 500};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
