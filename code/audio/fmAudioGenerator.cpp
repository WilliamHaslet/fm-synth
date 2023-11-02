#include "fmAudioGenerator.hpp"
#include "audioUtility.hpp"
#include <cmath>

float FMAudioGenerator::evaluate(ActiveNote& note, float time)
{
    float amplitude = 0;

    if (note.released)
    {
        amplitude = envelope->evaluateEnd(note.endTimer, note.amplitude);

        if (amplitude == 0)
        {
            note.done = true;
        }
    }
    else
    {
        amplitude = envelope->evaluateStart(note.startTimer);
        note.amplitude = amplitude;
    }

    float fmFrequency = midiNoteFrequency(note.note);
    float fmDepth = 20;
    float phase = sin(2 * PI * fmFrequency * time) * fmDepth;
    float frequency = 2 * PI * midiNoteFrequency(note.note);
    return sin((frequency * time) + phase) * amplitude;
}
