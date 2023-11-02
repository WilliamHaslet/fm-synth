#pragma once

enum MidiMessageType
{
    None,
    NoteOn,
    NoteOff,
    CC
};

struct MidiMessage
{
    MidiMessageType type = MidiMessageType::None;
    int channel = 0;
    int note = 0;
    int velocity = 0;
    int cc = 0;
    int value = 0;
};
