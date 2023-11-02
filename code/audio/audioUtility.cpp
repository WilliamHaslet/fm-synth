#include "audioUtility.hpp"
#include <cmath>

float midiNoteFrequency(int note)
{
    return 440 * (pow(2, (note - 69.0) / 12.0));
}
