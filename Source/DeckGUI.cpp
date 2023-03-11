#include <filesystem>

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "Formatter.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                juce::AudioFormatManager& _formatManager,
                juce::AudioThumbnailCache& cacheToUse
           ) : player(_player),
               formatManager(_formatManager),
               waveformSlider(_formatManager, cacheToUse)
{
    addAndMakeVisible(volumeFader);
    addAndMakeVisible(speedFader);

    addAndMakeVisible(loadButton);
    addAndMakeVisible(playControlButton);
    addAndMakeVisible(durationLabel);

    addAndMakeVisible(titleLabel);

    addAndMakeVisible(waveformSlider);

    volumeFader.addChangeListener(this);
    speedFader.addChangeListener(this);
    playControlButton.addChangeListener(this);
    player->addChangeListener(this);

    waveformSlider.addListener(this);
    loadButton.addListener(this);

    // Configure the load button appearance and behaviour
    juce::Image loadButtonImg = juce::ImageCache::getFromMemory(BinaryData::ejectbutton_png, BinaryData::ejectbutton_pngSize);
    loadButton.setImages(true, false, true,
        loadButtonImg, 1.0f, juce::Colours::transparentBlack,
        loadButtonImg, 0.7f, juce::Colours::transparentBlack,
        loadButtonImg, 0.4f, juce::Colours::transparentBlack);

    loadButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    loadButton.setTooltip("Load a file to the deck");
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void DeckGUI::resized()
{

    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    juce::Grid grid;

    grid.templateRows = { Track(), Track(Fr(1)) };
    grid.templateColumns = { Track(), Track(), Track(), Track(), Track(), Track(Fr(1)) };

    grid.items.addArray({
        /**
         * +-----------------+-----------------+------------+-------------------+---------------+-------------+
         * | volumeFader     | speedFader      | loadButton | playControlButton | durationLabel | titleLabel  |
         * +                 +                 +------------+-------------------+---------------+-------------+
         * |                 |                 | waveformSlider                                               |
         * +-----------------+-----------------+------------+-------------------------------------------------+
        */
        juce::GridItem(volumeFader).withWidth(50).withArea(1, 1, 4, 1),
        juce::GridItem(speedFader).withWidth(50).withArea(1, 2, 4, 2),

        juce::GridItem(loadButton).withSize(32, 32).withArea(1, 3).withMargin(juce::GridItem::Margin(5, 5, 5, 5)),
        juce::GridItem(playControlButton).withSize(32, 32).withArea(1, 4).withMargin(juce::GridItem::Margin(5, 5, 5, 0)),
        juce::GridItem(durationLabel).withWidth(50).withArea(1, 5),
        

        juce::GridItem(titleLabel).withArea(1, 6),

        juce::GridItem(waveformSlider).withArea(2, 3, 2, 7),
    });

    grid.performLayout(getLocalBounds());
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        std::cout << formatManager.getWildcardForAllFormats() << std::endl;
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
        fileChooser.launchAsync(juce::FileBrowserComponent::canSelectFiles, [this](const juce::FileChooser& chooser)
        {
            if (chooser.getResult().existsAsFile())
                loadFile(chooser.getResult());
        });
    }
}

void DeckGUI::sliderValueChanged (juce::Slider *slider)
{
    if (slider == &waveformSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

void DeckGUI::timerCallback()
{
    if (player->isPlaying()) {
        waveformSlider.setValue(player->getPositionRelative());
        durationLabel.setText(Formatter::formatTime(player->getLengthInSeconds() - player->getPositionAbsolute()), juce::dontSendNotification);
    }
}

/**
 * Overides the juce::FileDragAndDropTarget, this method is called when the user drags files over the component
 * we check if the file is supported by the format manager before allowing them to be dropped
 */
bool DeckGUI::isInterestedInFileDrag (const juce::StringArray &files)
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
void DeckGUI::filesDropped (const juce::StringArray &files, int x, int y)
{
    // at least one file
    if (files.size() < 1)
        return;

    // we can load only one file at a time into the deck, so we only check the first file
    juce::File file{files[0]};
    loadFile(file);
}

/** implement juce::TextDragAndDropTarget */
bool DeckGUI::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
    juce::ValueTree draggedItemInfo = juce::ValueTree::fromXml(dragSourceDetails.description.toString());
    if (draggedItemInfo.isValid() && draggedItemInfo.getType().toString() == "LibraryItem")
        return true;

    return false;
}

void DeckGUI::itemDropped(const SourceDetails& dragSourceDetails)
{
    juce::ValueTree draggedItemInfo = juce::ValueTree::fromXml(dragSourceDetails.description.toString());
    juce::String filePath = draggedItemInfo.getProperty("filePath");

    juce::File file{filePath};
    loadFile(file);
}

void DeckGUI::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &volumeFader)
    {
        player->setGain(volumeFader.getValue());
    }

    if (source == &speedFader)
    {
        player->setSpeed(speedFader.getValue());
    }

    if (source == &playControlButton)
    {
        if (playControlButton.getStatus() == PlayControlButton::Status::PlayRequested)
            player->start();
        else if (playControlButton.getStatus() == PlayControlButton::Status::PauseRequested)
            player->stop();
    }

    if (source == player)
    {
        if (player->isPlaying())
        {
            startTimer(500);
            playControlButton.setStatus(PlayControlButton::Status::Playing);
        }
        else
        {
            playControlButton.setStatus(PlayControlButton::Status::Paused);
            stopTimer();
        }
        waveformSlider.setValue(player->getPositionRelative());
    }
}

void DeckGUI::loadFile(juce::File file)
{
    if (!file.existsAsFile())
        return;

    juce::URL url{file};
    try {
        player->loadURL(url);
    } catch (DJAudioPlayer::UnsupportedFormatError e) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon,
            "Unsupported file format",
            "The file you are trying to load is not supported by the application.",
            "OK");
        return;
    }

    // Trying to get the metadata from the file
    auto metadata = player->metaData;
    if (metadata.size() > 0)
        titleLabel.setText(metadata["title"] + " by " + metadata["artist"], juce::dontSendNotification);
    else
        // Default values for the metadata
        titleLabel.setText(file.getFileNameWithoutExtension() + " by " + "Unknown artist", juce::dontSendNotification);

    durationLabel.setText(Formatter::formatTime(player->getLengthInSeconds()), juce::dontSendNotification);

    waveformSlider.loadURL(url);
    playControlButton.setStatus(PlayControlButton::Status::Paused);
}
