/**
 * This is a helper class for formatting time to have a
 * consistent look across the application.
*/
#pragma once

#include <JuceHeader.h>

class Formatter
{
public:
    Formatter();
    ~Formatter();

    /**
     * Format a time in milliseconds to a string.
     *
     * @param milliseconds The time in milliseconds.
     * @return The formatted time.
    */
    static juce::String formatTime(juce::int64 milliseconds);
};
