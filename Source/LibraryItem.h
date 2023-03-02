/*
  ==============================================================================

    LibraryItem.h
    Created: 26 Feb 2023 11:19:04pm
    Author:  Dzmitry Plashchynski

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct LibraryItem
{
    juce::String filePath;
    juce::String title;
    juce::String artist;

    // duration in seconds
    juce::int64 duration;
};
