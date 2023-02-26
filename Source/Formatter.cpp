/*
  ==============================================================================

    Formatter.cpp
    Created: 26 Feb 2023 11:26:32pm
    Author:  Dzmitry Plashchynski

  ==============================================================================
*/

#include "Formatter.h"

Formatter::Formatter()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

Formatter::~Formatter()
{
}

juce::String Formatter::formatTime(juce::int64 seconds)
{
    juce::int64 minutes = seconds / 60;
    juce::String timeString = juce::String::formatted("%02d:%02d", minutes % 60, seconds % 60);

    return timeString;
}

