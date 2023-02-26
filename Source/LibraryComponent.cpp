#include <iostream>
#include <filesystem>

#include <JuceHeader.h>
#include "LibraryComponent.h"
#include "LibraryItem.h"
#include "Formatter.h"

LibraryComponent::LibraryComponent(juce::AudioFormatManager& _formatManager) :
                        formatManager(_formatManager)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    tableComponent.getHeader().addColumn("Track", 1, 400);
    tableComponent.getHeader().addColumn("Artist", 2, 200);
    tableComponent.getHeader().addColumn("Duration", 3, 100);
    tableComponent.getHeader().addColumn("", 4, 200);
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
}

LibraryComponent::~LibraryComponent()
{
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
    // This method is where you should set the bounds of any child
    // components that your component contains..
    tableComponent.setBounds(getLocalBounds());
}

int LibraryComponent::getNumRows()
{
    return items.size();
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

    LibraryItem item = items[rowNumber];
    
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

juce::Component* LibraryComponent::refreshComponentForCell(int rowNumber, int columnId,
                                            bool isRowSelected,
                                            juce::Component* existingComponentToUpdate)
{
    if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* button = new juce::TextButton("Play");
            juce::String id{std::to_string(rowNumber)};
            button->setComponentID(id);
            button->addListener(this);
            existingComponentToUpdate = button;
        }
    }
    return existingComponentToUpdate;
}

void LibraryComponent::buttonClicked (juce::Button* button)
{
    int id = std::stoi(button->getComponentID().toStdString());
    std::cout << "Button clicked " << items[id].title << std::endl;
}

// virtual methods from FileDragAndDropTarget
// Check if the file is supported using AudioFormatManager to find the format by the file extension
bool LibraryComponent::isInterestedInFileDrag (const juce::StringArray& files)
{
    std::cout << "LibraryComponent::isInterestedInFileDrag" << std::endl;

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
    item.file_path = filePath;
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

    items.push_back(item);

    // Update the table
    tableComponent.updateContent();
}
