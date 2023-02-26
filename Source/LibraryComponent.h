#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

class LibraryComponent  :   public juce::Component,
                            public juce::TableListBoxModel,
                            public juce::Button::Listener,
                            public juce::FileDragAndDropTarget
{
public:
    LibraryComponent(juce::AudioFormatManager& formatManager);
    ~LibraryComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

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


private:
    juce::AudioFormatManager& formatManager;

    juce::TableListBox tableComponent;
    std::vector<std::string> trackTitles;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryComponent)
};
