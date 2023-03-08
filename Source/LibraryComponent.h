#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

class LibraryComponent  :   public juce::Component,
                            public juce::TableListBoxModel,
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
    juce::var getDragSourceDescription (const juce::SparseSet<int>& currentlySelectedRows) override;

    // virtual methods from FileDragAndDropTarget
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;

    // virtual methods from TextEditor::Listener
    void textEditorTextChanged(juce::TextEditor & editor) override;

    /**
     * Add a file to the library
     * @param filePath path to the file
    */
    void addFile(const juce::String& filePath);

    /**
     * Search items in the library by title and artist and display only the matching items
     * @param query query to filter the library
    */
    // std::vector<LibraryItem> searchLibrary(const juce::String & query);

    /**
     * Save the library to a file
    */
    void saveLibrary();

    /**
     * Load the library from a file
    */
    void loadLibrary();

private:
    juce::AudioFormatManager& formatManager;
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
    /**
     * Update the itemsToDisplay vector based on the search query
     * Should be called every time after the search query is changed or a new item is added to the library
    */
    void updateDisplayedItems();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryComponent)
};
