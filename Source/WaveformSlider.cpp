#include <JuceHeader.h>
#include "WaveformSlider.h"

//==============================================================================
WaveformSlider::WaveformSlider( juce::AudioFormatManager & 	formatManagerToUse,
                                juce::AudioThumbnailCache & 	cacheToUse) :
                                    juce::Slider(juce::Slider::SliderStyle::LinearBar, juce::Slider::NoTextBox),
                                    audioThumb(1000, formatManagerToUse, cacheToUse),
                                    fileLoaded(false)
{
    setLookAndFeel(&lookAndFeel);
    setRange(0.0, 1.0);
    setValue(0.0);
    audioThumb.addChangeListener(this);
}

WaveformSlider::~WaveformSlider()
{
    // This is necessary to avoid an assertion failure in the juce::LookAndFeel destructor
    setLookAndFeel(nullptr);
}

void WaveformSlider::loadURL(juce::URL audioURL)
{
    audioThumb.clear();
    fileLoaded  = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded)
    {
        setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
        setValue(0.0f);
        repaint();
    }
}

void WaveformSlider::changeListenerCallback (juce::ChangeBroadcaster *source)
{
    repaint();
}

// It's a like a paint() method for a Component
void WaveformSlider::LookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                       float sliderPos,
                                       float minSliderPos,
                                       float maxSliderPos,
                                       const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    g.fillAll(findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::lightblue);

    if (!fileLoaded)
    {
        g.setFont(20.0f);
        g.drawFittedText("No file loaded", x, y, width, height, juce::Justification::centred, 1);
        return;
    }

    juce::Rectangle<int> thumbArea(x, y, width, height);
    audioThumb.drawChannel(g, thumbArea, 0.0, audioThumb.getTotalLength(), 0, 1.0f);

    g.setColour(juce::Colour(30, 128, 239));
    g.drawLine(sliderPos, y, sliderPos, y + height, 3.0f);
    g.drawArrow(juce::Line<float>(sliderPos, y-5, sliderPos, y+15), 3.0f, 15.0f, 15.0f);
}
