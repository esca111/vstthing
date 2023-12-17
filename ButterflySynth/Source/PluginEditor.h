/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "midiKbrd.h"
#include "ADSRSlider.h"

//==============================================================================
/**
*/
class ButterflySynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ButterflySynthAudioProcessorEditor (ButterflySynthAudioProcessor&);
    ~ButterflySynthAudioProcessorEditor() override;
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    midiKbrd keyboardComponent;
    ADSRSlider adsrSlider;
    juce::Image backgroundImage;
    juce::ImageComponent overlay;

    //OSCS
    juce::Slider o1volSlider;
    juce::Slider o2volSlider;
    juce::Slider o3volSlider;
    juce::Slider o4volSlider;
    juce::Slider o5volSlider;
    juce::Slider o6volSlider;
    juce::Slider o7volSlider;
    juce::Slider o8volSlider;

    //osc attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> o1vAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> o2vAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> o3vAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> o4vAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> o5vAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> o6vAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> o7vAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> o8vAttachment;

    //ADSR
    juce::Slider atkSlider;
    juce::Slider dkSlider;
    juce::Slider sustainSlider;
    juce::Slider relSlider;

    //adsr attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> atkAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dkAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> relAttachment;

    //audioprocessor
    ButterflySynthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButterflySynthAudioProcessorEditor)
};
