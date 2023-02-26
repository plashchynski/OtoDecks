#include <iostream>
#include <filesystem>

#include <JuceHeader.h>
#include "LibraryComponent.h"

LibraryComponent::LibraryComponent(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");

    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("", 2, 200);
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
    return trackTitles.size();
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

    g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
}

juce::Component* LibraryComponent::refreshComponentForCell(int rowNumber, int columnId,
                                            bool isRowSelected,
                                            juce::Component* existingComponentToUpdate)
{
    if (columnId == 2)
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
    std::cout << "Button clicked " << trackTitles[id] << std::endl;
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
        std::cout << file.toStdString() << std::endl;
    }
}
