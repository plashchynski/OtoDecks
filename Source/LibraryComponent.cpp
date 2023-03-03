#include <iostream>
#include <filesystem>
#include <cassert>

#include <JuceHeader.h>
#include "LibraryComponent.h"
#include "LibraryItem.h"
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
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

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

void LibraryComponent::paintRowBackground(juce::Graphics& g, int rowNumber,
                            int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);
    else if (rowNumber % 2)
        g.fillAll(juce::Colours::lightgrey);
    else
        g.fillAll(juce::Colours::darkgrey);
}

void LibraryComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool rowIsSelected)
{
    g.setColour(juce::Colours::black);
    g.setFont(14.0f);

    LibraryItem item = itemsToDisplay[rowNumber];

    switch(columnId) {
        case 1:
            g.drawText(item.title, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        break;
        case 2:
            g.drawText(item.artist, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        break;
        case 3:
            g.drawText(Formatter::formatTime(item.duration), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        break;
    }
}

void LibraryComponent::buttonClicked (juce::Button* button)
{
    int id = std::stoi(button->getComponentID().toStdString());
    std::cout << "Button clicked " << itemsToDisplay[id].title << std::endl;
}

// virtual methods from FileDragAndDropTarget
// Check if the file is supported using AudioFormatManager to find the format by the file extension
bool LibraryComponent::isInterestedInFileDrag (const juce::StringArray& files)
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

void LibraryComponent::filesDropped (const juce::StringArray& files, int x, int y)
{
    std::cout << "LibraryComponent::filesDropped" << std::endl;
    for (auto file : files)
    {
        addFile(file.toStdString());
    }
}

/**
 * Add a file to the library
*/
void LibraryComponent::addFile(const std::string& filePath)
{
    std::cout << "Adding " << filePath << " to the library." << filePath << std::endl;

    juce::File audioFile(filePath);
    LibraryItem item;

    // Set default values
    item.filePath = filePath;
    item.title = audioFile.getFileNameWithoutExtension();
    item.artist = "Unknown";
    item.duration = 0;

    // Trying to get the metadata from the file
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(audioFile));
    if (reader)
    {
        item.duration = reader->lengthInSamples / reader->sampleRate;

        auto metadata = reader->metadataValues;
        if (metadata.size() > 0)
        {
            item.title = metadata["title"];
            item.artist = metadata["author"];
        }
    }

    allItems.push_back(item);

    // Update the itemsToDisplay vector based on the search query and with a new item
    updateDisplayedItems();

    // Save the library to the file
    saveLibrary();

    // Update the table
    tableComponent.updateContent();
}

// virtual methods from TextEditor::Listener
void LibraryComponent::textEditorTextChanged(juce::TextEditor& editor)
{
    if (&editor == &searchBox)
    {
        // Update the table content based on the search query
        updateDisplayedItems();
        tableComponent.updateContent();
    }
}

std::vector<LibraryItem> LibraryComponent::searchLibrary(const juce::String & query)
{
    std::vector<LibraryItem> itemsToDisplay;

    for (auto item : allItems)
    {
        if (item.title.containsIgnoreCase(query) || item.artist.containsIgnoreCase(query))
        {
            itemsToDisplay.push_back(item);
        }
    }

    return itemsToDisplay;
}

/**
 * Update the itemsToDisplay vector based on the search query
 * Should be called every time after the search query is changed or a new item is added to the library
*/
void LibraryComponent::updateDisplayedItems()
{
    itemsToDisplay = allItems;
    if (searchBox.getText().isNotEmpty())
    {
        itemsToDisplay = searchLibrary(searchBox.getText());
    }
}

juce::var LibraryComponent::getDragSourceDescription(const juce::SparseSet<int>& currentlySelectedRows)
{
    juce::ValueTree draggedItemInfo{"LibraryItem"};
    draggedItemInfo.setProperty("filePath", itemsToDisplay[currentlySelectedRows[0]].filePath, nullptr);

    return draggedItemInfo.toXmlString();
}

void LibraryComponent::saveLibrary()
{
    juce::Identifier libraryId("Library");
    juce::ValueTree libraryInfo{libraryId};

    for (int i=0; i<allItems.size(); i++)
    {
        LibraryItem item = allItems[i];
        juce::ValueTree itemInfo{"LibraryItem"+juce::String(i)};
        itemInfo.setProperty("filePath", item.filePath, nullptr);
        itemInfo.setProperty("title", item.title, nullptr);
        itemInfo.setProperty("artist", item.artist, nullptr);
        itemInfo.setProperty("duration", item.duration, nullptr);

        std::cout << "Saving item: " << itemInfo.getProperty("filePath").toString() << std::endl;

        libraryInfo.addChild(itemInfo, i, nullptr);
    }

    juce::File file{juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("OtoDecksLibrary.otdl")};
    juce::FileOutputStream output(file);

    std::cout << "Saving the library to the file: " << file.getFullPathName() << std::endl;

    if (!output.openedOk()) {
        std::cerr << "ERROR: Failed to open file for writing" << std::endl;
        return;
    }

    // Rewrite the whole file with a new data
    output.setPosition(0);
    output.truncate();

    assert(libraryInfo.isValid());

    libraryInfo.writeToStream(output);
    output.flush();

    std::cout << "Library saved." << std::endl;
}

void LibraryComponent::loadLibrary()
{
    juce::File file{juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("OtoDecksLibrary.otdl")};
    if (!file.existsAsFile())
    {
        std::cout << "Library file does not exist." << std::endl;
        return;
    }

    std::cout << "Loading library from the file: " << file.getFullPathName() << std::endl;

    juce::FileInputStream input(file);
    juce::ValueTree libraryInfo = juce::ValueTree::readFromStream(input);

    assert(libraryInfo.isValid());

    for (auto itemInfo : libraryInfo)
    {
        LibraryItem item;
        item.filePath = itemInfo.getProperty("filePath");
        item.title = itemInfo.getProperty("title");
        item.artist = itemInfo.getProperty("artist");
        item.duration = itemInfo.getProperty("duration");

        allItems.push_back(item);
    }

    updateDisplayedItems();

    tableComponent.updateContent();

    std::cout << "Library loaded" << std::endl;
}
