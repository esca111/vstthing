/*
  ==============================================================================

    sineWaveTable.cpp
    Created: 11 Aug 2023 11:52:25am
    Author:  

  ==============================================================================
*/

#include "sineWaveTable.h"
//this object allows for the use of wavetables for the oscillators instead of manually using std::sin as using sin uses a lot of processing power.
void sineWaveTable::setFrequency(float frequency, float sampleRate){
    auto cyclesPerSample = tableSize / sampleRate;
    tableDelta = frequency*cyclesPerSample;
}

void sineWaveTable::setLevel(float lvl1){
    tableLevel = lvl1;
}

