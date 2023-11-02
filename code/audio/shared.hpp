#pragma once
#include "midi.hpp"
#include "audio.hpp"
#include "audioGenerator.hpp"
#include <queue>
#include <vector>

struct SharedData
{
    long int frames = 0;
    std::queue<MidiMessage> messages;
    std::vector<ActiveNote> activeNotes;
    AudioGenerator* generator = NULL;
};
