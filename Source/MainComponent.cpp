#include <algorithm>

#include "MainComponent.h"

MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

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
    addAndMakeVisible(mixerControlPanel);
    addAndMakeVisible(libraryComponent);

    libraryComponent.loadLibrary();

    formatManager.registerBasicFormats();
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
     * The layout is:
     *
     * +-------------------+
     * | mixerControlPanel | height is fixed to 70px and cannot grow or shrink
     * +-------------------+
     * | Deck 1            | height is fixed to 150px and cannot grow or shrink
     * +-------------------+
     * | Deck 2            |
     * +-------------------+
     * | Library           | height is not fixed and can grow or shrink
     * |                   |
     * |                   |
     * +-------------------+
    */
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::column;

    fb.items.add(juce::FlexItem(mixerControlPanel).withMinHeight(70.0f).withFlex(0, 0));
    for (auto deck : decks)
    {
        fb.items.add(juce::FlexItem(*deck).withMinHeight(150.0f).withFlex(0, 0));
    }
    fb.items.add(juce::FlexItem(libraryComponent).withMinHeight(300.0f).withFlex(2, 2));

    fb.performLayout(getLocalBounds().toFloat());
}
