/*
  ==============================================================================

    midiKbrd.h
    Created: 27 May 2023 9:59:20pm
    Author:  

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//custom midikeyboard

class midiKbrd : public juce::MidiKeyboardComponent
{
public:
    midiKbrd(juce::MidiKeyboardState& state, Orientation orientation);

    virtual void drawBlackNote(int midiNoteNumber, juce::Graphics& g, juce::Rectangle<float> area, bool isDown, bool isOver, juce::Colour noteFillColour) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (midiKbrd)
};
