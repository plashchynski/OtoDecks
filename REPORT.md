# OOP Final Project Report

## Requirements

| Requirements        | Implemented?         |
| :--- | :---: |
| *R1: Basic functionality* | ✅ Implemented   |
| R1A: can load audio files into audio players   |   ✅ Implemented                   |
| R1B: can play two or more tracks    | ✅ Implemented |
| R1C: can mix the tracks by varying each of their volumes | ✅ Implemented |
| R1D: can speed up and slow down the tracks | ✅ Implemented |
| *R2: Custom deck control* | ✅ Implemented |
| R2A: Component has custom graphics implemented in a paint function | ✅ Implemented |
| R2B: Component enables the user to control the playback of a deck somehow | ✅ Implemented |
| *R3: Music library component* | ✅ Implemented |
| R3A: Component allows the user to add files to their library | ✅ Implemented |
| R3B: Component parses and displays meta data such as filename and song length | ✅ Implemented |
| R3C: Component allows the user to search for files | ✅ Implemented |
| R3D: Component allows the user to load files from the library into a deck | ✅ Implemented |
| R3E: The music library persists so that it is restored when the user exits then restarts the application | ✅ Implemented |
| *R4: Implementation of a complete custom GUI* | ✅ Implemented |
| R4B: GUI layout includes the custom Component from R2 | ✅ Implemented |
| R4C: GUI layout includes the music library component fro R3 | ✅ Implemented |

[[Screenshot of the GUI]]

## R1: Basic functionality

### R1A: can load audio files into audio players

There are three ways to load audio files into the audio players:
1. Drag and drop files from the file explorer into the deck.

This feature is implemented by overriding the `isInterestedInFileDrag` and `filesDropped` methods of `juce::FileDragAndDropTarget` in the `Deck` class. The `isInterestedInFileDrag` method is checking if a dragged file has a valid audio file extension supported by the global `AudioFormatManager` instance. The `filesDropped` method is called when a file is dropped into the deck, calls the `Deck::loadFile` method to load the file into the deck. The `Deck::loadFile` calls `player->loadURL` to load the file into the player. The `Player::loadURL` raises a custom exception `UnsupportedFormatError` if it fails to load the file into the player. The exception is caught in the `Deck::loadFile` method and an alert message is shown to the user:

[[Screenshot with an error message]]

It prevents the user from loading unsupported files into the deck and gives a visual feedback to the user if the file is not supported.

2. Click the "Load" button in the deck and select a file from the file explorer.

[[Screenshot with buttons]]

It uses the `formatManager.getWildcardForAllFormats()` to get a wildcard string for all the supported audio file extensions. This string is passed to the `juce::FileChooser` constructor to filter the files in the file explorer. When a file is selected, the `Deck::loadFile` is called to load the file into the deck. So, the `Deck::loadFile` is a single point of entry for loading files into the deck and performs the same checks as in the first method.

3. Drag and drop library items from the library into the deck.

You can add tracks to the library and then drag and drop them into the deck to start playing. This is done by overriding the `isInterestedInDragSource` and `itemDropped` methods of `juce::DragAndDropTarget` in the `Deck` class. An library item made draggable by overriding the `getDragSourceDescription` method of `juce::TableListBoxModel` in the `LibraryComponent` class. The information about the item is stored in a XML-serialized `juce::ValueTree` object. The `isInterestedInDragSource` method of the `DecDecklass performs check to ensure that the dragged object is a library item. The `itemDropped` method calls the `Deck::loadFile` method to load the item into the deck.

[[Screenshot with library]]

### R1B: can play two or more tracks

To play a track it should be loaded into a deck. The application starts with two decks, but a user can add as many decks as needed. Actually, the number of simultaneously playing tracks are limited only by capacity of the computer. There's a button to add a new deck and buttons to remove decks:

[[Screenshot with three tracks playing]]

The deck is implemented in the `Deck` class. Every deck has its own playing backend implemented in `Player` class that encapsulate all audio-related complexity. The `MainComponent` is responsible for storing decks and managing them. The list of decks is stored in the `MainComponent` using a `juce::OwnedArray` class to automatically manage the lifetime of the decks. There are `addDeck` and the `removeDeck` methods in `MainComponent` to add and remove decks on user request.

### R1C: can mix the tracks by varying each of their volumes

### R1D: can speed up and slow down the tracks



## R2: Custom deck control

Implementation of a custom deck control Component with custom graphics which
allows the user to control deck playback in some way that is more advanced than stop/ start.

### R2A: Component has custom graphics implemented in a paint function

### R2B: Component enables the user to control the playback of a deck somehow



## R3: Music library component

Implementation of a music library component which allows the user to manage their
music library.

### R3A: Component allows the user to add files to their library

You can drag and drop files into the library. Only the valid audio files will be added to the library. This is done by checking the file extension in the `LibraryComponent` class in the `isInterestedInFileDrag` method. The `AudioFormatManager::findFormatForFileExtension` method is used to check if the file extension is supported by the global `AudioFormatManager` instance.

### R3B: Component parses and displays meta data such as filename and song length

When a file is added, the `LibraryComponent` class uses the `AudioFormatManager::createReaderFor` method to create an `AudioFormatReader` instance for the selected file. The `AudioFormatReader` instance is used to get the meta-information. The `LibraryComponent` uses a `LibraryItem` struct to store and display all the meta-information about the audio files.

### R3C: Component allows the user to search for files

The `LibraryComponent` class has a data member `searchBox` of a `TextEditor` class which is used to search for items in the library. We have two vectors of `LibraryItem`s in the `LibraryComponent` class. `allItems` contains all the items in the library and `itemsToDisplay` contains the items that should be displayed in the UI based on the search query or other factors.

If a user types in the search box, the `LibraryComponent` class receives a `TextEditor::textEditorTextChanged` callback. Inside the callback, we call `updateDisplayedItems` method which calls `searchLibrary` to find matched items in the `allItems` vector and then update the `itemsToDisplay` vector.

### R3D: Component allows the user to load files from the library into a deck

You can drag and drop files from the library into the deck. This is done by allowing a library item to be dragged by overriding the `juce::TableListBoxModel::getDragSourceDescription` method in the `LibraryComponent` class. An information about the dragged item is stored in a XML-serialized `juce::ValueTree` object. The dragged object received by overriding the `juce::TextDragAndDropTarget::itemDropped` method in the `Deck` class. The `DecDecknserializes the `juce::ValueTree` object from a XML string received by `itemDropped` as an argument, get the item's file path, finally loads the file into the deck.

### R3E: The music library persists so that it is restored when the user exits then restarts the application

This functionality is implemented in `LibraryComponent::saveLibrary` and `LibraryComponent::loadLibrary` methods. The `saveLibrary` method serializes the `allItems` vector into a `juce::ValueTree` object and then saves it to a binary file `OtoDecksLibrary.otdl` in a user's Documents directory. This is an OS-independent to save a file. The `loadLibrary` method loads the saved file and then deserializes the `juce::ValueTree` object into the `allItems` vector. The library saved each time a new file is added to the library. The library is loaded when the application starts.

## R4: Implementation of a complete custom GUI

### R4A: GUI layout is significantly different from the basic Deck shown in class, with extra controls

1. To ensure the app has enough space to display all the controls, I set the minimum size of the application window to `1024x768` pixels, but not less than the user's screen resolution. This is done by determining the screen size and then setting the minimum size for the `ComponentBoundsConstrainer` instance in the `MainWindow::MainWindow` method in `Main.cpp`:

    ```c++
        // Get the current screen size
        juce::Rectangle<int> screenArea = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea;

        // Set minimum height and width for the window in pixels
        const int minWidth = 1024;
        const int minHeight = 768;

        // Get the constrainer that determines the screen size limits
        juce::ComponentBoundsConstrainer* constrainer = getConstrainer();

        // The minimum size should not be less than the screen size
        constrainer->setMinimumWidth(juce::jmin(minWidth, screenArea.getWidth()));
        constrainer->setMinimumHeight(juce::jmin(minHeight, screenArea.getHeight()));
    ```

2. I've disabled a screen saver and a screen lock on Windows and macOS, which is a common practice for music applications. This is done by calling `juce::Desktop::getInstance().setScreenSaverEnabled(false)` in the `MainWindow::MainWindow` method in `Main.cpp`.

### R4B: GUI layout includes the custom Component from R2

### R4C: GUI layout includes the music library component fro R3
