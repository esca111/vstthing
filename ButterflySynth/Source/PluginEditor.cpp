/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "midiKbrd.h"
#include "ADSRSlider.h"

//==============================================================================
ButterflySynthAudioProcessorEditor::ButterflySynthAudioProcessorEditor (ButterflySynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent(p.kbState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    //this is all of the UI elements and their attachments if they're interactable.
    juce::Colour ADSRCol = juce::Colour(246, 104, 182);
    juce::Colour operatorCol = juce::Colour::fromRGBA(191, 130, 222, 100);
    setSize (900, 700);
    setResizable(false, false);
    addAndMakeVisible(keyboardComponent);
    keyboardComponent.setBounds(0, getHeight()-70, getWidth(), 70);

    //oscillator attachments
    o1vAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "o1v", o1volSlider);
    o2vAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "o2v", o2volSlider);
    o3vAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "o3v", o3volSlider);
    o4vAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "o4v", o4volSlider);
    o5vAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "o5v", o5volSlider);
    o6vAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "o6v", o6volSlider);
    o7vAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "o7v", o7volSlider);
    o8vAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "o8v", o8volSlider);
    
    //adsr attachments
    
    atkAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", atkSlider);
    atkSlider.setLookAndFeel(&adsrSlider);
    dkAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DECAY", dkSlider);
    dkSlider.setLookAndFeel(&adsrSlider);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);
    sustainSlider.setLookAndFeel(&adsrSlider);
    relAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", relSlider);
    relSlider.setLookAndFeel(&adsrSlider);
    
    //volume sliders for oscillators

    o1volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    o1volSlider.setTextBoxStyle(juce::Slider::NoTextBox , false, 20, 25);
    o1volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
    o1volSlider.setColour(juce::Slider::trackColourId, operatorCol);
    addAndMakeVisible(o1volSlider);

    o2volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    o2volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 20, 25);
    o2volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
    o2volSlider.setColour(juce::Slider::trackColourId, operatorCol);
    addAndMakeVisible(o2volSlider);

    o3volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    o3volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 20, 25);
    o3volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
    o3volSlider.setColour(juce::Slider::trackColourId, operatorCol);
    addAndMakeVisible(o3volSlider);

    o4volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    o4volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 20, 25);
    o4volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
    o4volSlider.setColour(juce::Slider::trackColourId, operatorCol);
    addAndMakeVisible(o4volSlider);

    o5volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    o5volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 20, 25);
    o5volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
    o5volSlider.setColour(juce::Slider::trackColourId, operatorCol);
    addAndMakeVisible(o5volSlider);

    o6volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    o6volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 20, 25);
    o6volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
    o6volSlider.setColour(juce::Slider::trackColourId, operatorCol);
    addAndMakeVisible(o6volSlider);

    o7volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    o7volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 20, 25);
    o7volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
    o7volSlider.setColour(juce::Slider::trackColourId, operatorCol);
    addAndMakeVisible(o7volSlider);
    
    o8volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    o8volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 20, 25);
    o8volSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
    o8volSlider.setColour(juce::Slider::trackColourId, operatorCol);
    addAndMakeVisible(o8volSlider);
    
    //adsr sliders

    atkSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    atkSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
    atkSlider.setColour(juce::Slider::trackColourId, ADSRCol);
    atkSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f));
    atkSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
    addAndMakeVisible(atkSlider);

    dkSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    dkSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
    dkSlider.setColour(juce::Slider::trackColourId, ADSRCol);
    dkSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f));
    dkSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
    addAndMakeVisible(dkSlider);

    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    sustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
    sustainSlider.setColour(juce::Slider::trackColourId, ADSRCol);
    sustainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f));
    sustainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
    addAndMakeVisible(sustainSlider);

    relSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    relSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
    relSlider.setColour(juce::Slider::trackColourId, ADSRCol);
    relSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f));
    relSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.0f));
    addAndMakeVisible(relSlider);
}

ButterflySynthAudioProcessorEditor::~ButterflySynthAudioProcessorEditor()
{
}

//==============================================================================
void ButterflySynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::BACKGROUND_png, BinaryData::BACKGROUND_pngSize);
    g.drawImageAt(background, 0, 0);
}

void ButterflySynthAudioProcessorEditor::resized()
{
    keyboardComponent.setBounds(0, getHeight() - 70, getWidth(), 70);

    const auto padding = 32;
    const auto OscSliderWidth = 40;
    const auto ADSRSliderWidth = 20;
    const auto sliderHeight = 158;

    atkSlider.setBounds(52, 400, ADSRSliderWidth, sliderHeight);
    dkSlider.setBounds(atkSlider.getRight() + padding + 1, 400, ADSRSliderWidth, sliderHeight);
    sustainSlider.setBounds(dkSlider.getRight() + padding + 1, 400, ADSRSliderWidth, sliderHeight);
    relSlider.setBounds(sustainSlider.getRight() + padding, 400, ADSRSliderWidth, sliderHeight);
    overlay.setBounds(0, 0, 900, 700);

    o1volSlider.setBounds(24, 122, OscSliderWidth, 217);
    o2volSlider.setBounds(o1volSlider.getRight(), 122, OscSliderWidth, 217);
    o3volSlider.setBounds(o2volSlider.getRight(), 122, OscSliderWidth, 217);
    o4volSlider.setBounds(o3volSlider.getRight(), 122, OscSliderWidth, 217);
    o5volSlider.setBounds(o4volSlider.getRight(), 122, OscSliderWidth, 217);
    o6volSlider.setBounds(o5volSlider.getRight(), 122, OscSliderWidth, 217);
    o7volSlider.setBounds(o6volSlider.getRight(), 122, OscSliderWidth, 217);
    o8volSlider.setBounds(o7volSlider.getRight(), 122, OscSliderWidth, 217);
}

