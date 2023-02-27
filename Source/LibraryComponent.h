#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

#include "LibraryItem.h"

class LibraryComponent  :   public juce::Component,
                            public juce::TableListBoxModel,
                            public juce::Button::Listener,
                            public juce::FileDragAndDropTarget,
                            public juce::TextEditor::Listener
{
public:
    LibraryComponent(juce::AudioFormatManager& formatManager);
    ~LibraryComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // virtual methods from TableListBoxModel
    int getNumRows() override;
    void paintRowBackground(juce::Graphics& g, int rowNumber,
                            int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool rowIsSelected) override;

    juce::Component* refreshComponentForCell(int rowNumber, int columnId,
                                            bool isRowSelected,
                                            juce::Component* existingComponentToUpdate) override;
    void buttonClicked (juce::Button* button) override;

    // virtual methods from FileDragAndDropTarget
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;


    // virtual methods from TextEditor::Listener
    void textEditorTextChanged(juce::TextEditor & editor) override;

    /**
     * Add a file to the library
     * @param filePath path to the file
    */
    void addFile(const std::string& filePath);

    /**
     * Search items in the library by title and artist and display only the matching items
     * @param query query to filter the library
    */
    std::vector<LibraryItem> searchLibrary(const juce::String & query);

private:
    juce::AudioFormatManager& formatManager;
    juce::TableListBox tableComponent;

    /**
     * List of all items in the library (unfiltered)
    */
    std::vector<LibraryItem> allItems;

    /**
     * List of items to display in the tableComponent based on the search query
    */
    std::vector<LibraryItem> itemsToDisplay;

    juce::TextEditor searchBox;

    /**
     * Update the itemsToDisplay vector based on the search query
     * Should be called every time after the search query is changed or a new item is added to the library
    */
    void updateDisplayedItems();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryComponent)
};
