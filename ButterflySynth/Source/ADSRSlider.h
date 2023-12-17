/*
  ==============================================================================

    ADSRSlider.h
    Created: 16 Jul 2023 8:16:31pm
    Author:  

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ADSRSlider : public juce::LookAndFeel_V4 {
public:
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider) override;
};