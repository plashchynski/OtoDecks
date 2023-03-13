#include <algorithm>

#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (800, 600);

    addAndMakeVisible(addDeckButton);
    addAndMakeVisible(masterVolumeFader);
    addAndMakeVisible(masterMuteButton);

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
    masterMuteButton.addChangeListener(this);

    addDeckButton.addListener(this);

    masterMuteButton.setMuted(false);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    for (auto deck : decks)
    {
        deck->player.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    for (auto deck : decks)
    {
        mixerSource.addInputSource(&deck->player, false);
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
    for (auto deck : decks)
    {
        deck->player.releaseResources();
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
     * +-----+-------------------+------------------+
     * | (+) | masterVolumeFader | masterMuteButton |
     * +-----+-------------------+------------------+
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
    mixerControlsFb.items.add(juce::FlexItem(masterMuteButton).withMargin(10).withMinWidth(50).withFlex(0, 0));

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
    Deck *deck = new Deck(formatManager, thumbCache);
    decks.add(deck);

    addAndMakeVisible(deck);
    deck->addChangeListener(this);

    mixerSource.addInputSource(&deck->player, false);

    resized();
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &masterVolumeFader)
        juce::SystemAudioVolume::setGain(masterVolumeFader.getValue());

    else if (typeid(*source) == typeid(Deck))
    {
        Deck *deck = dynamic_cast<Deck *>(source);
        if (deck->toBeRemoved)
            removeDeck(deck);
    }
    else if (source == &masterMuteButton)
    {
        juce::SystemAudioVolume::setMuted(masterMuteButton.isMuted());
        resized();
    }
}

void MainComponent::buttonClicked(juce::Button *button)
{
    if (button == &addDeckButton)
        addDeck();
}

void MainComponent::removeDeck(Deck *deck)
{
    // Remove the deck
    removeChildComponent(deck);
    mixerSource.removeInputSource(&deck->player);
    decks.removeObject(deck);

    resized();
}
