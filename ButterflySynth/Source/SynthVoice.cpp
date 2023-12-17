/*
  ==============================================================================

    SynthVoice.cpp
    Created: 27 May 2023 5:48:34pm
    Author:  

  ==============================================================================
*/

#include "SynthVoice.h"
#include <iostream>
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    for (auto oscillatorIndex = 0; oscillatorIndex < oscs.size(); ++oscillatorIndex){
        auto* oscillator = oscs.getUnchecked(oscillatorIndex);
        oscillator->setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber)*pow(2, oscillatorIndex), getSampleRate());
    };
    adsr.noteOn();
    std::cout << "startNote called" << std::endl;
}

void SynthVoice::createWaveTable(){
    sineTable.setSize(1, tableSize); //this function creates the wavetable used to make the synth more efficient
    auto* samples = sineTable.getWritePointer (0);

    auto angleDelta = juce::MathConstants<double>::twoPi / (double) (tableSize - 1);
    auto currentAngle = 0.0;

    for (unsigned int i = 0; i < tableSize; ++i)
    {
        auto sample = std::sin (currentAngle);                                  
        samples[i] = (float) sample;
        currentAngle += angleDelta;
    }
    samples[tableSize] = samples[0];
}

void SynthVoice::updateOscVolume(int index, float lvl) {
        auto* oscillator = oscs.getUnchecked(index);
        oscillator->tableLevel = lvl;
}

void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    adsr.noteOff();
    if (!allowTailOff && !adsr.isActive()) {
        clearCurrentNote();
    }
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerblock, int outputChannels) {
    if(hasPrepared){
        return;
    } //i have no idea why this worked. for some reason if this is exported as a vst, preparetoplay is called 3 times instead of 1 and it breaks the whole DAW. this works and i don't know or care how. i literally made this as a last ditch effort just to see if it would do something, but somehow it fixed the entire issue
    hasPrepared = true;
    adsr.setSampleRate(sampleRate);
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerblock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    createWaveTable();
    gain.prepare(spec);
    auto numberOfOscillators = 8;                                              
    for (auto i = 0; i < numberOfOscillators; i++){
        auto* oscillator = new sineWaveTable(sineTable);
        oscs.add(oscillator);
    }

    level = 0.25f / (float)numberOfOscillators;
    gain.setGainLinear(0.8f);
    isReady = true;
}

void SynthVoice::updateGain(const float newgain) {
    gain.setGainLinear(newgain*level);
}
void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release) {
    //adsr parameters
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;

    adsr.setParameters(adsrParams);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    //this renders the next audio block the voice is processing
    if (!isVoiceActive()) {
        return;
    }
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();
    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
    for (auto oscillatorIndex = 0; oscillatorIndex < oscs.size(); ++oscillatorIndex) {
        auto* oscillator = oscs.getUnchecked(oscillatorIndex);
        for(int j = startSample; j < audioBlock.getNumSamples(); j++){
            float temp = audioBlock.getSample(0, j); //if we don't make a temporary sample then each oscillator in the voice overrides the previous one
            float sample = oscillator->getNextTableSample();
            //put the sample into the audio buffer in both the left and right channel
            audioBlock.setSample(0, j, (temp+sample*oscillator->tableLevel));
            audioBlock.setSample(1, j, (temp+sample*oscillator->tableLevel));
        }
    };
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock)); //process the global gain
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    for (int i = 0; i < outputBuffer.getNumChannels(); i++) {
        outputBuffer.addFrom(i, startSample, synthBuffer, i, 0, numSamples);
        if (!adsr.isActive()) {
            clearCurrentNote(); //if the adsr isnt active reset the voice
        }
    }
}
