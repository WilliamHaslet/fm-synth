#pragma once
#include "audioGenerator.hpp"

class SineAudioGenerator : public AudioGenerator
{
public:
    float evaluate(ActiveNote& note, float time);
};
