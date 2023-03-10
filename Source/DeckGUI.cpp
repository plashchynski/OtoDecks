#include <filesystem>

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                juce::AudioFormatManager& _formatManager,
                juce::AudioThumbnailCache& cacheToUse
           ) : player(_player),
               formatManager(_formatManager),
               waveformSlider(_formatManager, cacheToUse)
{
    addAndMakeVisible(loadButton);
    addAndMakeVisible(playControlButton);

    // addAndMakeVisible(volLabel);
    // addAndMakeVisible(volValueLabel);
    // addAndMakeVisible(volSlider);
    addAndMakeVisible(volumeFader);

    addAndMakeVisible(speedLabel);
    addAndMakeVisible(speedValueLabel);
    addAndMakeVisible(speedSlider);

    addAndMakeVisible(titleLabel);
    addAndMakeVisible(artistLabel);

    addAndMakeVisible(waveformSlider);

    volumeFader.addChangeListener(this);

    // Set up the sliders and its labels
    // volLabel.setText("Volume", juce::dontSendNotification);
    speedLabel.setText("Speed", juce::dontSendNotification);

    // volSlider.addListener(this);
    speedSlider.addListener(this);
    waveformSlider.addListener(this);

    // volSlider.setRange(0.0, 1.0);
    // volSlider.setValue(1.0);

    speedSlider.setRange(0, 3.0);
    speedSlider.setValue(1.0);

    loadButton.addListener(this);

    playControlButton.addChangeListener(this);
    player->addChangeListener(this);

    juce::Image loadButtonImg = juce::ImageCache::getFromMemory(BinaryData::ejectbutton_png, BinaryData::ejectbutton_pngSize);
    loadButton.setImages(false, true, true,
        loadButtonImg, 1.0f, juce::Colour(0x00000000),
        loadButtonImg, 1.0f, juce::Colour(0x00000000),
        loadButtonImg, 1.0f, juce::Colour(0x00000000));
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

    grid.templateRows = { Track (Fr (1)), Track (Fr (1)), Track (Fr (3))  };
    grid.templateColumns = { Track(), Track(), Track(), Track(), Track(Fr(1)) };

    grid.items.addArray({
        /**
         * +-----------------+-----------------+------------+------------+
         * | speedFader      | volLabel        |            |            |
         * +                 +-----------------+------------+------------+
         * |                 | volValueLabel   |            |            |
         * +                 +-----------------+------------+------------+
         * |                 | volSlider       |            |            |
         * +-----------------+-----------------+------------+------------+
        */
        juce::GridItem(volumeFader).withWidth(100).withArea(1, 2, 4, 2),
        // juce::GridItem(volLabel).withArea(1, 2),

        juce::GridItem(speedValueLabel).withArea(2, 1),
        // juce::GridItem(volValueLabel).withArea(2, 2),

        juce::GridItem(speedSlider).withWidth(100).withArea(3, 1, 4, 1),
        // juce::GridItem(volSlider).withWidth(100).withArea(3, 2, 4, 2),

        juce::GridItem(playControlButton).withWidth(40).withArea(1, 3),
        juce::GridItem(loadButton).withWidth(40).withArea(1, 4),
        juce::GridItem(titleLabel).withArea(1, 5),
        juce::GridItem(artistLabel).withArea(2, 5),
        juce::GridItem(waveformSlider).withArea(3, 3, 4, 6),
    });

    grid.performLayout(getLocalBounds());
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        auto fileChooserFlags =
        juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
        {
            loadFile(chooser.getResult());
        });
    }
}

void DeckGUI::sliderValueChanged (juce::Slider *slider)
{
    // if (slider == &volSlider)
    // {
    //     player->setGain(slider->getValue());
    //     volValueLabel.setText(juce::String::formatted("%d%%", (int)(slider->getValue() * 100)), juce::dontSendNotification);
    // }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
        speedValueLabel.setText(juce::String::formatted("%d%%", (int)(slider->getValue() * 100)), juce::dontSendNotification);
    }

    if (slider == &waveformSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

void DeckGUI::timerCallback()
{
    if (player->isPlaying()) {
        waveformSlider.setValue(player->getPositionRelative());
    }
}

/** implement FileDragAndDropTarget */
bool DeckGUI::isInterestedInFileDrag (const juce::StringArray &files)
{
    for (auto file : files)
    {
        std::string extension = std::filesystem::path(file.toStdString()).extension();
        juce::AudioFormat* format = formatManager.findFormatForFileExtension(extension);
        if (format == nullptr)
            return false;
    }

    return true;
}

void DeckGUI::filesDropped (const juce::StringArray &files, int x, int y)
{
    if (files.size() > 0)
    {
        juce::File file{files[0]};
        loadFile(file);
    }
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

    titleLabel.setText(draggedItemInfo.getProperty("title"), juce::dontSendNotification);
    artistLabel.setText(draggedItemInfo.getProperty("artist"), juce::dontSendNotification);

    juce::File file{filePath};
    loadFile(file);
}

void DeckGUI::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &volumeFader)
    {
        player->setGain(volumeFader.getValue());
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
    juce::URL url{file};
    player->loadURL(url);
    waveformSlider.loadURL(url);
    playControlButton.setStatus(PlayControlButton::Status::Paused);
}
