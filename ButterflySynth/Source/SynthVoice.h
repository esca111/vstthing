/*
  ==============================================================================

    SynthVoice.h
    Created: 27 May 2023 5:48:34pm
    Author:  

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "sineWaveTable.h"
//this is for creating a voice (one note) sound that plays when a MIDI note is sent to the synth
class SynthVoice : public juce::SynthesiserVoice {
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerblock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void updateADSR(const float attack, const float decay, const float sustain, const float release);
    void updateGain(const float newgain); 
    void updateOscVolume(int index, float lvl);

private:
    juce::AudioBuffer<float> synthBuffer;
    juce::AudioSampleBuffer sineTable;
    const unsigned int tableSize = 1 << 9;
    void createWaveTable();
    float level;
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    int voiceNumber;
    juce::OwnedArray<sineWaveTable> oscs;
    juce::dsp::Gain<float> gain;
    bool isReady { false };
    bool hasPrepared { false };
};
