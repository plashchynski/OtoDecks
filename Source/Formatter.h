/*
  ==============================================================================

    Formatter.h
    Created: 26 Feb 2023 11:26:32pm
    Author:  Dzmitry Plashchynski

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Formatter
{
public:
    Formatter();
    ~Formatter();

    static juce::String formatTime(juce::int64 milliseconds);
};
