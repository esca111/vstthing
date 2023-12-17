/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>
//==============================================================================
ButterflySynthAudioProcessor::ButterflySynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    std::cout << "startup" << std::endl;
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());

}

ButterflySynthAudioProcessor::~ButterflySynthAudioProcessor()
{
}

//==============================================================================
const juce::String ButterflySynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ButterflySynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ButterflySynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ButterflySynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ButterflySynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ButterflySynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ButterflySynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ButterflySynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ButterflySynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void ButterflySynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ButterflySynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void ButterflySynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ButterflySynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    //boilerplate code
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ButterflySynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //this allows for the processing of audio blocks and subsequent audio output
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
   
    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) { //process audio for each voice
            //ADSR
            auto& atk = *apvts.getRawParameterValue("ATTACK");
            auto& dk = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& rel = *apvts.getRawParameterValue("RELEASE");
            voice->updateADSR(atk.load(), dk.load(), sustain.load(), rel.load());
            for (int j = 0; j < 8; j++) {
                std::string st = "o" + std::to_string(j + 1) + "v"; //this looks weird, but it allows each oscillator to be called without code-spam and allows for the list of oscillators to be expanded if need be.
                auto& volume = *apvts.getRawParameterValue(st);
                voice->updateOscVolume(j, volume.load());
            }
        }
    }

    juce::MidiBuffer incomingMidi; //if we don't use a separate midi buffer for the on-screen keyboard the program crashes on processNextMidiBuffer
    kbState.processNextMidiBuffer(incomingMidi, 0, buffer.getNumSamples(), true);
    midiMessages.addEvents(incomingMidi, 0, incomingMidi.getNumEvents(), 0);
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool ButterflySynthAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* ButterflySynthAudioProcessor::createEditor()
{
    return new ButterflySynthAudioProcessorEditor (*this);
}

//==============================================================================
void ButterflySynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState(); //this code restores the synth's state from when it was last used.
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ButterflySynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    //this saves the synths state to be called from getStateInformation() after the program has been closed and reopened.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
                if (xmlState->hasTagName (apvts.state.getType()))
                    apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}


// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ButterflySynthAudioProcessor();
}

//value tree

juce::AudioProcessorValueTreeState::ParameterLayout ButterflySynthAudioProcessor::createParams() {
    //create parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    //osc
    params.push_back(std::make_unique<juce::AudioParameterFloat>("o1v", "Operator 1 volume", juce::NormalisableRange<float>{0, 1.0f, 0.005f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("o2v", "Operator 2 volume", juce::NormalisableRange<float>{0, 1.0f, 0.005f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("o3v", "Operator 3 volume", juce::NormalisableRange<float>{0, 1.0f, 0.005f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("o4v", "Operator 4 volume", juce::NormalisableRange<float>{0, 1.0f, 0.005f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("o5v", "Operator 5 volume", juce::NormalisableRange<float>{0, 1.0f, 0.005f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("o6v", "Operator 6 volume", juce::NormalisableRange<float>{0, 1.0f, 0.005f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("o7v", "Operator 7 volume", juce::NormalisableRange<float>{0, 1.0f, 0.005f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("o8v", "Operator 8 volume", juce::NormalisableRange<float>{0, 1.0f, 0.005f}, 0.0f));
    //adsr
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>{0.005f, 1.0f, 0.005f}, 0.005f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>{0.005f, 1.0f, 0.005f}, 0.005f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>{0.005f, 1.0f, 0.005f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float>{0.05f, 1.0f, 0.005f}, 0.005f));

    return{ params.begin(), params.end() };
}
