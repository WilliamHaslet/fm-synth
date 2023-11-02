#include "sineAudioGenerator.hpp"
#include "audioUtility.hpp"
#include <cmath>

float SineAudioGenerator::evaluate(ActiveNote& note, float time)
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

    return sin(2 * PI * midiNoteFrequency(note.note) * time) * amplitude;
}
