/*
  ==============================================================================

    sineWaveTable.h
    Created: 11 Aug 2023 11:52:25am
    Author:  

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <iostream>
class sineWaveTable{
public:
    sineWaveTable(const juce::AudioSampleBuffer& wavetableToUse) : wavetable (wavetableToUse), tableSize (wavetable.getNumSamples() - 1)
    {};
    void createWaveTable();
    void setFrequency (float frequency, float sampleRate);
    forcedinline float getNextTableSample() noexcept {
        //this function gets the next sample to play when the oscillator requires it.
        auto index0 = (unsigned int) currentIndex;
        auto index1 = index0 + 1;
        auto frac = currentIndex - (float) index0;
        auto *table = wavetable.getReadPointer(0);
        auto value0 = table[index0];
        auto value1 = table[index1];
        auto currentSample = value0 + (frac * (value1 - value0));
        if ((currentIndex += tableDelta) > (float) tableSize)
            //this allows for the wavetable to loop in order to actually generate a frequency and not reference an index outside the range of the wavetable
            currentIndex = fmod(currentIndex, tableSize);
        return currentSample;
    };
    void setLevel(float lvl1);
    float tableLevel;
private:
    float currentIndex = 0.0f, tableDelta = 0.0f;
    const juce::AudioSampleBuffer& wavetable;
    const int tableSize;
};

