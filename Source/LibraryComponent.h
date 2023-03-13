/**
 * This class represents a library — a component that displays a list of audio files (items) and
 * allows the user to add new files, search for files, and drag and drop files to the decks
*/
#pragma once

#include <JuceHeader.h>
#include <vector>

#include "ControlButton.h"

class LibraryComponent  :   public juce::Component,
                            public juce::TableListBoxModel,
                            public juce::FileDragAndDropTarget,
                            public juce::TextEditor::Listener,
                            public juce::Button::Listener
{
public:
    /**
     * Constructor
     *
     * @param formatManager a reference to a global format manager
    */
    LibraryComponent(juce::AudioFormatManager& formatManager);
    ~LibraryComponent() override;

    // juce::Component overrides
    void paint (juce::Graphics&) override;
    void resized() override;

    // implementing juce::TableListBoxModel
    int getNumRows() override;
    void paintRowBackground(juce::Graphics& g, int rowNumber,
                            int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool rowIsSelected) override;
    juce::var getDragSourceDescription (const juce::SparseSet<int>& currentlySelectedRows) override;
    juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected,
                                                juce::Component* existingComponentToUpdate) override;

    // implementing juce::FileDragAndDropTarget
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;

    // implementing juce::TextEditor::Listener
    void textEditorTextChanged(juce::TextEditor & editor) override;

    // implementing juce::Button::Listener
    void buttonClicked(juce::Button* button) override;

    /**
     * Add a file to the library
     * @param filePath path to the file
    */
    void addFile(const juce::String& filePath);

    /**
     * Save the library to a file
    */
    void saveLibrary();

    /**
     * Load the library from a file
    */
    void loadLibrary();

private:
    // A reference to a global format manager
    juce::AudioFormatManager& formatManager;

    // A table component to display the library items
    juce::TableListBox tableComponent;

    // This is a value tree that will be used to store the library data
    juce::ValueTree library{"Library"};

    /**
     * Vector of items to display in the table
     * It may be different from the library value tree if the user is searching for something
    */
    std::vector<juce::ValueTree> itemsToDisplay = {};

    /**
     * Search box to filter the library items by title and artist
    */
    juce::TextEditor searchBox;

    // A button to add a file to the library
    ControlButton addButton{juce::ImageCache::getFromMemory(BinaryData::plus_png, BinaryData::plus_pngSize), "Add file to the library"};

    /**
     * Update the itemsToDisplay vector based on the search query
     * Should be called every time after the search query is changed or a new item is added to the library
    */
    void updateDisplayedItems();

    //  A file chooser to select a file to add to the library
    juce::FileChooser fileChooser{"Select a file...", juce::File(), formatManager.getWildcardForAllFormats()};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryComponent)
};
