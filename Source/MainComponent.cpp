#include <algorithm>

#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (800, 600);

    addAndMakeVisible(addDeckButton);
    addAndMakeVisible(masterVolumeFader);

    for (int i = 0; i < numDecks; i++)
        addDeck();

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);
    }

    // This is required to make tooltips work in the components
    addAndMakeVisible(tooltipWindow);
    addAndMakeVisible(libraryComponent);

    libraryComponent.loadLibrary();

    formatManager.registerBasicFormats();

    masterVolumeFader.addChangeListener(this);

    addDeckButton.addListener(this);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    for (auto player : players)
    {
        player->prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    for (auto player : players)
    {
        mixerSource.addInputSource(player, false);
    }
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    for (auto player : players)
    {
        player->releaseResources();
    }
    mixerSource.releaseResources();
}

void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    /**
     * 
     * The layout of mixerControlsFb is:
     * 
     * +-----+-------------------+
     * | (+) | masterVolumeFader |
     * +-----+-------------------+
     * 
     * 
     * The layout of mainFb is:
     *
     * +-------------------+
     * | mixerControlsFb   | height is fixed to 70px and cannot grow or shrink
     * +-----+-------------+
     * | (-) | Deck 1      | height is fixed to 150px and cannot grow or shrink
     * +-----+-------------+
     * | (-) | Deck 2      |
     * +-----+-------------+
     * | Library           | height is not fixed and can grow or shrink
     * |                   |
     * |                   |
     * +-------------------+
     * 
     * 
    */

    juce::FlexBox mixerControlsFb;
    mixerControlsFb.flexDirection = juce::FlexBox::Direction::row;
    mixerControlsFb.items.add(juce::FlexItem(addDeckButton).withMargin(10).withMinWidth(50).withFlex(0, 0));
    mixerControlsFb.items.add(juce::FlexItem(masterVolumeFader).withMinWidth(150.0f).withFlex(0, 0));

    juce::FlexBox mainFb;
    mainFb.flexDirection = juce::FlexBox::Direction::column;

    mainFb.items.add(juce::FlexItem(mixerControlsFb).withHeight(70).withFlex(0, 0));
    for (auto deck : decks)
        mainFb.items.add(juce::FlexItem(*deck).withHeight(150).withFlex(0, 0));

    mainFb.items.add(juce::FlexItem(libraryComponent).withMinHeight(300.0f).withFlex(2, 2));

    mainFb.performLayout(getLocalBounds().toFloat());
}

void MainComponent::addDeck()
{
    DJAudioPlayer *player = new DJAudioPlayer(formatManager);
    DeckGUI *deck = new DeckGUI(player, formatManager, thumbCache);

    players.push_back(player);
    decks.push_back(deck);
    addAndMakeVisible(deck);
    deck->addChangeListener(this);

    mixerSource.addInputSource(player, false);

    resized();
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &masterVolumeFader)
        juce::SystemAudioVolume::setGain(masterVolumeFader.getValue());

    else if (typeid(*source) == typeid(DeckGUI))
    {
        DeckGUI *deck = dynamic_cast<DeckGUI*>(source);
        if (deck->toBeRemoved)
        {
            removeDeck(deck);
        }
    }
}

void MainComponent::buttonClicked(juce::Button *button)
{
    if (button == &addDeckButton)
        addDeck();
}

void MainComponent::removeDeck(DeckGUI *deck)
{
    // Find the index of the deck
    int index = std::find(decks.begin(), decks.end(), deck) - decks.begin();

    // Remove the deck
    removeChildComponent(deck);
    decks.erase(decks.begin() + index);
    delete deck;

    // Stop and delete the player
    DJAudioPlayer *player = players[index];
    mixerSource.removeInputSource(player);
    players.erase(players.begin() + index);
    player->releaseResources();
    delete player;

    resized();
}
