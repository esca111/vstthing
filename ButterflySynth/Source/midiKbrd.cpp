/*
  ==============================================================================

    midiKbrd.cpp
    Created: 27 May 2023 9:59:20pm
    Author:  

  ==============================================================================
*/

#include <JuceHeader.h>
#include "midiKbrd.h"

//==============================================================================
midiKbrd::midiKbrd(juce::MidiKeyboardState &state, Orientation orientation) : MidiKeyboardComponent(state, orientation)
{
    int numChildren = getNumChildComponents();
    for (int i = 0; i < numChildren; i++) {
        Component* child = getChildComponent(i);
        child->setWantsKeyboardFocus(false);
    }
}

void midiKbrd::drawBlackNote(int midiNoteNumber, juce::Graphics& g, juce::Rectangle<float> area, bool isDown, bool isOver, juce::Colour noteFillColour) {
    juce::Colour colour(noteFillColour);
    
    if (isDown)
        colour = colour.overlaidWith(findColour(keyDownOverlayColourId));

    if (isOver)
        colour = colour.overlaidWith(findColour(mouseOverKeyOverlayColourId));

    g.setColour(colour);
    g.fillRect(area);
}
