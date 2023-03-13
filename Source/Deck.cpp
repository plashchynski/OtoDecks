#include <filesystem>

#include <JuceHeader.h>
#include "Deck.h"
#include "Formatter.h"

Deck::Deck( juce::AudioFormatManager& _formatManager,
            juce::AudioThumbnailCache& cacheToUse) :
            player(_formatManager),
            formatManager(_formatManager),
            waveformSlider(_formatManager, cacheToUse)
{
    // add all child components to display
    addAndMakeVisible(volumeFader);
    addAndMakeVisible(speedFader);
    addAndMakeVisible(removeButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(playControlButton);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(durationLabel);
    addAndMakeVisible(titleLabel);
    addAndMakeVisible(waveformSlider);

    // listen to the events from the child components
    volumeFader.addChangeListener(this);
    speedFader.addChangeListener(this);
    playControlButton.addChangeListener(this);
    muteButton.addChangeListener(this);
    player.addChangeListener(this);

    waveformSlider.addListener(this);
    loadButton.addListener(this);
    removeButton.addListener(this);

    // set the appearance of the child components
    durationLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    titleLabel.setFont(juce::Font(16.0f, juce::Font::plain));
}

Deck::~Deck()
{
    // release the resources used by the deck before it is destroyed
    player.releaseResources();
    stopTimer();
}

void Deck::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void Deck::resized()
{
    // layout the child components using juce::Grid

    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    juce::Grid grid;

    grid.templateRows = { Track(), Track(Fr(2)) };
    grid.templateColumns = { Track(), Track(), Track(), Track(), Track(), Track(), Track(), Track(Fr(1)) };

    grid.items.addArray({
        /**
         * +-----+-----------------+-----------------+------------+-------------------+------------+---------------+-------------+
         * | (X) | volumeFader     | speedFader      | loadButton | playControlButton | muteButton | durationLabel | titleLabel  |
         * |     +                 +                 +------------+-------------------+------------+---------------+-------------+
         * |     |                 |                 | waveformSlider                                                            |
         * +-----+-----------------+-----------------+---------------------------------------------------------------------------+
        */
        juce::GridItem(removeButton).withWidth(50).withArea(1, 1).withMargin(juce::GridItem::Margin(5, 10, 5, 10)),
        juce::GridItem(volumeFader).withWidth(50).withArea(1, 2, 4, 2),
        juce::GridItem(speedFader).withWidth(50).withArea(1, 3, 4, 3),

        juce::GridItem(loadButton).withSize(32, 32).withArea(1, 4).withMargin(juce::GridItem::Margin(5, 5, 5, 5)),
        juce::GridItem(playControlButton).withSize(32, 32).withArea(1, 5).withMargin(juce::GridItem::Margin(5, 5, 5, 0)),
        juce::GridItem(muteButton).withSize(32, 32).withArea(1, 6).withMargin(juce::GridItem::Margin(5, 5, 5, 0)),
        juce::GridItem(durationLabel).withWidth(50).withArea(1, 7),
        juce::GridItem(titleLabel).withArea(1, 8),

        juce::GridItem(waveformSlider).withArea(2, 4, 2, 9),
    });

    grid.performLayout(getLocalBounds());
}

void Deck::buttonClicked(juce::Button* button)
{
    // handle button clicks
    if (button == &loadButton)
    {
        // open a file chooser to select a file to load
        fileChooser.launchAsync(juce::FileBrowserComponent::canSelectFiles, [this](const juce::FileChooser& chooser)
        {
            if (chooser.getResult().existsAsFile())
                loadFile(chooser.getResult());
        });
    }
    else if (button == &removeButton)
    {
        // prepare the deck to be removed
        player.stop();
        toBeRemoved = true;
        // signal the parent component that the deck is to be removed
        sendChangeMessage();
    }
}

void Deck::sliderValueChanged (juce::Slider *slider)
{
    // intercommunicate the waveform slider and the player
    if (slider == &waveformSlider)
        player.setPositionRelative(slider->getValue());
}

void Deck::timerCallback()
{
    // periodically update the information about the playback
    if (player.isPlaying()) {
        waveformSlider.setValue(player.getPositionRelative());
        durationLabel.setText(Formatter::formatTime(player.getLengthInSeconds() - player.getPositionAbsolute()), juce::dontSendNotification);
    }
}

/**
 * Overides the juce::FileDragAndDropTarget, this method is called when the user drags files over the component
 * we check if the file is supported by the format manager before allowing them to be dropped
 */
bool Deck::isInterestedInFileDrag (const juce::StringArray &files)
{
    // we can load only one file at a time into the deck, so we only check the first file
    auto file = files[0];
    std::string extension = std::filesystem::path(file.toStdString()).extension();

    // check if the file extension is supported by the format manager
    juce::AudioFormat* format = formatManager.findFormatForFileExtension(extension);
    if (format == nullptr)
        return false;

    return true;
}

// Overrides the juce::FileDragAndDropTarget, this method is called when the user drops files on the component
void Deck::filesDropped (const juce::StringArray &files, int x, int y)
{
    // at least one file
    if (files.size() < 1)
        return;

    // we can load only one file at a time into the deck, so we only check the first file
    juce::File file{files[0]};
    loadFile(file);
}

/** implement juce::TextDragAndDropTarget */
bool Deck::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
    juce::ValueTree draggedItemInfo = juce::ValueTree::fromXml(dragSourceDetails.description.toString());
    if (draggedItemInfo.isValid() && draggedItemInfo.getType().toString() == "LibraryItem")
        return true;

    return false;
}

/** implement juce::TextDragAndDropTarget
 *
 * it is used to load a file from the library
*/
void Deck::itemDropped(const SourceDetails& dragSourceDetails)
{
    juce::ValueTree draggedItemInfo = juce::ValueTree::fromXml(dragSourceDetails.description.toString());
    juce::String filePath = draggedItemInfo.getProperty("filePath");

    juce::File file{filePath};
    loadFile(file);
}

// implement juce::ChangeListener to receive notifications from the child components
void Deck::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    // Change volume (gain)
    if (source == &volumeFader)
    {
        player.setGain(volumeFader.getValue());
    }

    // Change speed
    else if (source == &speedFader)
    {
        player.setSpeed(speedFader.getValue());
    }

    // play/pause the player
    else if (source == &playControlButton)
    {
        if (playControlButton.getStatus() == PlayControlButton::Status::PlayRequested)
            player.start();
        else if (playControlButton.getStatus() == PlayControlButton::Status::PauseRequested)
            player.stop();
    }

    // update the play/pause button and the waveform slider when the player changes state
    else if (source == &player)
    {
        if (player.isPlaying())
        {
            startTimer(500);
            playControlButton.setStatus(PlayControlButton::Status::Playing);
        }
        else
        {
            playControlButton.setStatus(PlayControlButton::Status::Paused);
            stopTimer();
        }
        waveformSlider.setValue(player.getPositionRelative());
    }

    // Mute the deck
    else if (source == &muteButton)
    {
        muteButton.isMuted() ? player.setGain(0) : player.setGain(volumeFader.getValue());
    }
}

/**
 * One point of entry for loading a file into the deck
*/
void Deck::loadFile(juce::File file)
{
    if (!file.existsAsFile())
        return;

    juce::URL url{file};

    // Here we are using player to check if the file format is supported:
    try {
        player.loadURL(url);
    } catch (Player::UnsupportedFormatError e) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon,
            "Unsupported file format",
            "The file you are trying to load is not supported by the application.",
            "OK");
        return;
    }

    // Trying to get the metadata from the file
    auto metadata = player.metaData;
    if (metadata.size() > 0)
        titleLabel.setText(metadata["title"] + " by " + metadata["artist"], juce::dontSendNotification);
    else
        // Default values for the metadata
        titleLabel.setText(file.getFileNameWithoutExtension() + " by " + "Unknown artist", juce::dontSendNotification);

    durationLabel.setText(Formatter::formatTime(player.getLengthInSeconds()), juce::dontSendNotification);

    waveformSlider.loadURL(url);

    // It's paused by default
    playControlButton.setStatus(PlayControlButton::Status::Paused);
}
