#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "LibraryComponent.h"
#include "Fader.h"
#include "ControlButton.h"

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

    // Implements juce::ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    // Implements juce::Button::Listener
    void buttonClicked(juce::Button *button) override;

private:
    void addDeck();
    void removeDeck(int index);

    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{100};

    std::vector<DJAudioPlayer*> players;
    std::vector<DeckGUI*> decks;
    std::vector<ControlButton*> removeDeckButtons;

    juce::MixerAudioSource mixerSource;

    ControlButton addDeckButton{
        juce::ImageCache::getFromMemory(BinaryData::plus_png, BinaryData::plus_pngSize),
        "Add an additional deck"
    };

    juce::Image removeDeckButtonImg = juce::ImageCache::getFromMemory(BinaryData::remove_png, BinaryData::remove_pngSize);

    Fader masterVolumeFader{"Master Volume", Fader::Type::Horizontal, "percent", 0, 1, 1};

    LibraryComponent libraryComponent{formatManager};

    juce::TooltipWindow tooltipWindow{this, 500};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
