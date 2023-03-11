#include <algorithm>

#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (800, 600);

    addAndMakeVisible(addDeckButton);
    addAndMakeVisible(masterVolumeFader);

    int numPlayers = 2;
    for (int i = 0; i < numPlayers; i++)
    {
        players.push_back(new DJAudioPlayer(formatManager));
        decks.push_back(new DeckGUI(players[i], formatManager, thumbCache));
        addAndMakeVisible(decks[i]);
    }

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

    // Configure the add deck button appearance and behaviour
    juce::Image addDeckButtonImg = juce::ImageCache::getFromMemory(BinaryData::plus_png, BinaryData::plus_pngSize);
    addDeckButton.setImages(true, false, true,
        addDeckButtonImg, 1.0f, juce::Colours::transparentBlack,
        addDeckButtonImg, 0.7f, juce::Colours::transparentBlack,
        addDeckButtonImg, 0.4f, juce::Colours::transparentBlack);

    addDeckButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    addDeckButton.setTooltip("Add an additional deck");
    addDeckButton.addListener(this);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
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
     * The layout of mainFb is:
     *
     * +-------------------+
     * | mixerControlsFb   | height is fixed to 70px and cannot grow or shrink
     * +-------------------+
     * | Deck 1            | height is fixed to 150px and cannot grow or shrink
     * +-------------------+
     * | Deck 2            |
     * +-------------------+
     * | Library           | height is not fixed and can grow or shrink
     * |                   |
     * |                   |
     * +-------------------+
     * 
     * 
     * The layout of mixerControlsFb is:
     * 
     * +---------------+-------------------+
     * | addDeckButton | masterVolumeFader |
     * +---------------+-------------------+
     * 
    */

    juce::FlexBox mixerControlsFb;
    mixerControlsFb.flexDirection = juce::FlexBox::Direction::row;
    mixerControlsFb.items.add(juce::FlexItem(addDeckButton).withMargin(10).withMinWidth(50.0f).withFlex(0, 0));
    mixerControlsFb.items.add(juce::FlexItem(masterVolumeFader).withMinWidth(150.0f).withFlex(0, 0));

    juce::FlexBox mainFb;
    mainFb.flexDirection = juce::FlexBox::Direction::column;

    mainFb.items.add(juce::FlexItem(mixerControlsFb).withMinHeight(70.0f).withFlex(0, 0));
    for (auto deck : decks)
    {
        mainFb.items.add(juce::FlexItem(*deck).withMinHeight(150.0f).withFlex(0, 0));
    }
    mainFb.items.add(juce::FlexItem(libraryComponent).withMinHeight(300.0f).withFlex(2, 2));

    mainFb.performLayout(getLocalBounds().toFloat());
}

void MainComponent::addDeck()
{
    players.push_back(new DJAudioPlayer(formatManager));
    decks.push_back(new DeckGUI(players.back(), formatManager, thumbCache));
    addAndMakeVisible(decks.back());
    resized();
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &masterVolumeFader)
    {
        juce::SystemAudioVolume::setGain(masterVolumeFader.getValue());
    }
}

void MainComponent::buttonClicked(juce::Button *button)
{
    if (button == &addDeckButton)
    {
        addDeck();
    }
}
