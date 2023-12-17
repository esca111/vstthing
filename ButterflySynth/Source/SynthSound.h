/*
  ==============================================================================

    SynthSound.h
    Created: 27 May 2023 5:48:25pm
    Author:  

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound {
//boilerplate code required for SynthVoice
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    
    bool appliesToChannel(int midiChannel) override { return true; }
};
