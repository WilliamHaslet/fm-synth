#pragma once
#include "audioGenerator.hpp"

class FMAudioGenerator : public AudioGenerator
{
public:
    float evaluate(ActiveNote& note, float time);
};
