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

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformSlider);

    loadButton.addListener(this);

    playControlButton.addChangeListener(this);
    player->addChangeListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    waveformSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void DeckGUI::resized()
{
    playControlButton.setBounds(0, 0, playControlButton.getWidth(), playControlButton.getHeight());
    waveformSlider.setBounds(playControlButton.getWidth(), 0, getWidth(), getHeight());
    // double rowH = getHeight() / 8;
    // volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    // speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    // posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
    // waveformSlider.setBounds(0, rowH * 5, getWidth(), rowH * 2);
    // loadButton.setBounds(0, rowH * 7, getWidth(), rowH);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        auto fileChooserFlags =
        juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
        {
            player->loadURL(juce::URL{chooser.getResult()});
            // and now the waveformSlider as well
            waveformSlider.loadURL(juce::URL{chooser.getResult()});
        });
    }
}

void DeckGUI::sliderValueChanged (juce::Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
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
        {
            std::cout << extension << " file extension is not suported" << std::endl;
            return false;
        }
    }

    return true;
}

void DeckGUI::filesDropped (const juce::StringArray &files, int x, int y)
{
    if (files.size() > 0)
    {
        juce::File file{files[0]};
        juce::URL url{file};

        player->loadURL(url);
        waveformSlider.loadURL(url);
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

    juce::File file{filePath};
    juce::URL url{file};
    player->loadURL(url);
    waveformSlider.loadURL(url);
}

void DeckGUI::changeListenerCallback(juce::ChangeBroadcaster *source)
{
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
            playControlButton.setPlaying(true);
        else
            playControlButton.setPlaying(false);

        waveformSlider.setValue(player->getPositionRelative());
    }
}
