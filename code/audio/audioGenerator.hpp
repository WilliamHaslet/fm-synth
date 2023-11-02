#pragma once
#include "audio.hpp"

class AudioGenerator
{
public:
    ADSR* envelope = 0;
    virtual float evaluate(ActiveNote& note, float time) = 0;
};
