/*
  ==============================================================================

    ADSRSlider.cpp
    Created: 16 Jul 2023 8:16:31pm
    Author:  

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRSlider.h"
//this is for creating the rounded sliders in the ADSR menu
void ADSRSlider::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos,
    float minSliderPos,
    float maxSliderPos,
    const juce::Slider::SliderStyle style, juce::Slider& slider)
    
    {
    if (slider.isBar()) {
        g.setColour(slider.findColour(juce::Slider::trackColourId));
        g.fillRoundedRectangle(slider.isHorizontal() ? juce::Rectangle<float>(static_cast<float> (x), (float)y + 0.5f, sliderPos - (float)x, (float)height - 1.0f)
           : juce::Rectangle<float>((float)x + 0.5f, sliderPos, (float)width - 1.0f, (float)y + ((float)height - sliderPos)), 9);
        //g.fillRoundedRectangle(slider.getLocalBounds().toFloat().reduced(0.5f), 10);
    }
}
