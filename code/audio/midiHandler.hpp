#pragma once
#include "RtMidi.h"
#include "shared.hpp"

class MidiHandler
{
public:
	SharedData* sharedData = 0;

private:
	RtMidiIn* midiIn = 0;

public:
	MidiHandler();
	~MidiHandler();
};
