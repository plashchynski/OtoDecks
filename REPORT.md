# OOP Final Project Report

## Requirements

✅ — means that the requirement is implemented

❌ — means that the requirement is not implemented

### R1: Basic functionality ✅

The application should contain all the basic functionality shown in class:

#### R1A: can load audio files into audio players ✅

#### R1B: can play two or more tracks ✅ 


#### R1C: can mix the tracks by varying each of their volumes ✅


#### R1D: can speed up and slow down the tracks ✅



### R2: Custom deck control ❌

Implementation of a custom deck control Component with custom graphics which
allows the user to control deck playback in some way that is more advanced than stop/ start.

#### R2A: Component has custom graphics implemented in a paint function ❌

#### R2B: Component enables the user to control the playback of a deck somehow ❌



### R3: Music library component ❌

Implementation of a music library component which allows the user to manage their
music library.

#### R3A: Component allows the user to add files to their library ✅

You can drag and drop files into the library. Only the valid audio files will be added to the library. This is done by checking the file extension in the `LibraryComponent` class in the `isInterestedInFileDrag` method. The `AudioFormatManager::findFormatForFileExtension` method is used to check if the file extension is supported by the global `AudioFormatManager` instance.

#### R3B: Component parses and displays meta data such as filename and song length ✅

When a file is added, the `LibraryComponent` class uses the `AudioFormatManager::createReaderFor` method to create an `AudioFormatReader` instance for the selected file. The `AudioFormatReader` instance is used to get the meta-information. The `LibraryComponent` uses a `LibraryItem` struct to store and display all the meta-information about the audio files.

#### R3C: Component allows the user to search for files ❌

#### R3D: Component allows the user to load files from the library into a deck ❌

#### R3E: The music library persists so that it is restored when the user exits then restarts the application ❌


### R4: Implementation of a complete custom GUI ❌

#### R4A: GUI layout is significantly different from the basic DeckGUI shown in class, with extra controls ❌

#### R4B: GUI layout includes the custom Component from R2 ❌

#### R4C: GUI layout includes the music library component fro R3 ❌
