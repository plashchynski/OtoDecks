/**
 * This class represents a main window of the application with main components:
 * - decks
 * - library
*/

#pragma once

#include <JuceHeader.h>
#include "Player.h"
#include "Deck.h"
#include "LibraryComponent.h"
#include "Fader.h"
#include "ControlButton.h"
#include "MuteButton.h"

class MainComponent :   public juce::AudioAppComponent,
                        public juce::DragAndDropContainer,
                        public juce::ChangeListener,
                        public juce::Button::Listener
{
public:
    // Constructor
    MainComponent();
    ~MainComponent() override;

    // Implements juce::AudioAppComponent
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // Implements juce::Component
    void paint (juce::Graphics& g) override;
    void resized() override;

    // Implements juce::ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    // Implements juce::Button::Listener
    void buttonClicked(juce::Button *button) override;

private:
    // default number of decks to create on startup
    const int numDecks = 2;

    // A global audio format manager
    juce::AudioFormatManager formatManager;

    // A global audio thumbnail cache
    juce::AudioThumbnailCache thumbCache{100};

    // A global audio source used to mix sources from all decks
    juce::MixerAudioSource mixerSource;

    // Add and remove decks
    void addDeck();
    void removeDeck(Deck *deck);

    // A list of decks
    juce::OwnedArray<Deck> decks;

    // A button to add a new deck
    ControlButton addDeckButton{
        juce::ImageCache::getFromMemory(BinaryData::plus_png, BinaryData::plus_pngSize),
        "Add an additional deck"
    };

    // Master volume fader
    Fader masterVolumeFader{"Master Volume", Fader::Type::Horizontal, "percent", 0, 1, 1};

    // Master mute button
    MuteButton masterMuteButton;

    // The library component
    LibraryComponent libraryComponent{formatManager};

    // This is required to show tooltips in all child components
    juce::TooltipWindow tooltipWindow{this, 500};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
