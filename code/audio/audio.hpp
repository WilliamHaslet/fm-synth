#pragma once
#include "../utility.hpp"
#include "../control.hpp"
#include "../controls.hpp"
#include "audioUtility.hpp"

struct ActiveNote
{
    int note = 0;
    int velocity = 0;
    float startTimer = 0;
    float endTimer = 0;
    float amplitude = 0;
    bool released = false;
    bool done = false;
};

struct ADSR
{
    Control* attackControl = 0;
    Control* decayControl = 0;
    Control* sustainControl = 0;
    Control* releaseControl = 0;

    void newControl(Controls& controls, Control*& ptr, std::string name, float startingValue)
    {
        ptr = controls.newControl(name);
        ptr->setValue(startingValue);
    }

    ADSR(Controls& controls)
    {
        newControl(controls, attackControl, "attack", 0.01);
        newControl(controls, decayControl, "decay", 0.1);
        newControl(controls, sustainControl, "sustain", 1);
        newControl(controls, releaseControl, "release", 0.2);
    }

    float evaluateStart(float time)
    {
        float amplitude = 0;
        float attack = remap(attackControl->getValue(), 0, 1, 0.001, 2);
        float decay = remap(decayControl->getValue(), 0, 1, 0.001, 2);
        float sustain = sustainControl->getValue();

        if (inRange(time, 0, attack))
        {
            amplitude = remap(time, 0, attack, 0, 1);
        }
        else if (inRange(time, attack, attack + decay))
        {
            amplitude = remap(time, attack, attack + decay, 1, sustain);
        }
        else if (time > attack + decay)
        {
            amplitude = sustain;
        }

        return clamp(amplitude, 0, 1);
    }

    float evaluateEnd(float time, float startingAmplitude)
    {
        float amplitude = 0;
        float release = remap(releaseControl->getValue(), 0, 1, 0.001, 5);

        if (inRange(time, 0, release))
        {
            amplitude = remap(time, 0, release, startingAmplitude, 0);
        }

        return clamp(amplitude, 0, 1);
    }
};
