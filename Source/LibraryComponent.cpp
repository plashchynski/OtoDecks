#include <iostream>
#include <filesystem>
#include <cassert>

#include <JuceHeader.h>
#include "LibraryComponent.h"
#include "Formatter.h"

LibraryComponent::LibraryComponent(juce::AudioFormatManager& _formatManager) :
                        formatManager(_formatManager)
{
    tableComponent.getHeader().addColumn("Track", 1, 400);
    tableComponent.getHeader().addColumn("Artist", 2, 200);
    tableComponent.getHeader().addColumn("Duration", 3, 100);
    tableComponent.getHeader().addColumn("", 4, 200);
    tableComponent.setModel(this);

    addAndMakeVisible(searchBox);
    addAndMakeVisible(tableComponent);

    searchBox.addListener(this);
    searchBox.setTextToShowWhenEmpty("Search for a track or an artist...", juce::Colours::grey);

    updateDisplayedItems();
}

LibraryComponent::~LibraryComponent()
{
    /**
     * to avoid an assertion in juce_ListBox.cpp:63
     * More information on this issue: https://forum.juce.com/t/issue-with-tablelistbox-and-accessibility/53866
    */
    tableComponent.setModel(nullptr);
}

void LibraryComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("LibraryComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void LibraryComponent::resized()
{
    searchBox.setBounds(0, 0, getWidth(), 30);
    tableComponent.setBounds(0, 30, getWidth(), getHeight() - 30);
}

int LibraryComponent::getNumRows()
{
    return itemsToDisplay.size();
}

void LibraryComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool rowIsSelected)
{
    g.setColour(juce::Colours::black);
    g.setFont(14.0f);

    juce::ValueTree item = itemsToDisplay[rowNumber];

    switch(columnId) {
        case 1:
            g.drawText(item.getProperty("title"), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        break;
        case 2:
            g.drawText(item.getProperty("artist"), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        break;
        case 3:
            g.drawText(Formatter::formatTime(item.getProperty("duration")), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        break;
    }
}

void LibraryComponent::paintRowBackground(juce::Graphics& g, int rowNumber,
                            int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);
    else if (rowNumber % 2)
        g.fillAll(juce::Colours::lightgrey);
    else
        g.fillAll(juce::Colours::grey);
}

// Check if the file is supported using AudioFormatManager to find the format by the file extension
bool LibraryComponent::isInterestedInFileDrag (const juce::StringArray& files)
{
    for (auto file : files)
    {
        std::string extension = std::filesystem::path(file.toStdString()).extension();
        juce::AudioFormat* format = formatManager.findFormatForFileExtension(extension);
        if (format == nullptr)
            return false; // The format is not supported
    }

    return true;
}

/**
 * File dropped on the component
*/
void LibraryComponent::filesDropped (const juce::StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        addFile(file);
    }
}

/**
 * Add a file to the library
*/
void LibraryComponent::addFile(const juce::String& filePath)
{
    std::cout << "Adding " << filePath << " to the library." << filePath << std::endl;

    juce::File audioFile(filePath);

    juce::ValueTree itemInfo("LibraryItem");
    itemInfo.setProperty("filePath", filePath, nullptr);
    itemInfo.setProperty("title", audioFile.getFileNameWithoutExtension(), nullptr);
    itemInfo.setProperty("artist", "Unknown", nullptr);
    itemInfo.setProperty("duration", 0, nullptr);

    // Trying to get the metadata from the file
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(audioFile));
    if (reader)
    {
        itemInfo.setProperty("duration", reader->lengthInSamples / reader->sampleRate, nullptr);

        auto metadata = reader->metadataValues;
        if (metadata.size() > 0)
        {
            itemInfo.setProperty("title", metadata["title"], nullptr);
            itemInfo.setProperty("artist", metadata["artist"], nullptr);
        }
    }

    library.appendChild(itemInfo, nullptr);

    // Save the library to the file
    saveLibrary();

    // Update the itemsToDisplay vector based on the search query and with a new item
    updateDisplayedItems();

    // Update the table
    tableComponent.updateContent();
}

// virtual methods from TextEditor::Listener
void LibraryComponent::textEditorTextChanged(juce::TextEditor& editor)
{
    if (&editor == &searchBox)
    {
        updateDisplayedItems();
        tableComponent.updateContent();
    }
}

/**
 * Update the itemsToDisplay vector based on the search query
 * Should be called every time after the search query is changed or a new item is added to the library
*/
void LibraryComponent::updateDisplayedItems()
{
    itemsToDisplay.clear();

    if (searchBox.getText().isEmpty())
    {
        for (auto item : library)
        {
            itemsToDisplay.push_back(item);
        }
        return;
    }

    for (auto item : library)
    {
        if (item.getProperty("title").toString().containsIgnoreCase(searchBox.getText()) ||
            item.getProperty("artist").toString().containsIgnoreCase(searchBox.getText()))
        {
            itemsToDisplay.push_back(item);
        }
    }
}

juce::var LibraryComponent::getDragSourceDescription(const juce::SparseSet<int>& currentlySelectedRows)
{
    return itemsToDisplay[currentlySelectedRows[0]].toXmlString();
}

void LibraryComponent::saveLibrary()
{
    juce::File file{juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("OtoDecksLibrary.otdl")};
    juce::FileOutputStream output(file);

    if (!output.openedOk()) {
        std::cerr << "ERROR: Failed to open file for writing: " << file.getFullPathName() << std::endl;
        return;
    }

    // Rewrite the whole file with a new data
    output.setPosition(0);
    output.truncate();

    assert(library.isValid());

    library.writeToStream(output);
    output.flush();
}

void LibraryComponent::loadLibrary()
{
    juce::File file{juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("OtoDecksLibrary.otdl")};
    if (!file.existsAsFile())
        return;

    juce::FileInputStream input(file);
    library = juce::ValueTree::readFromStream(input);

    assert(library.isValid());

    updateDisplayedItems();
    tableComponent.updateContent();
}
