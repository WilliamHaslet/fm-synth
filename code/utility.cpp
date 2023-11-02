#include "utility.hpp"

std::string roundedFloatString(float number, int decimalPlaces, int maxLength)
{
    char* charArray = new char[maxLength];
    snprintf(charArray, maxLength, "%.*f", decimalPlaces, number);
    std::string string = std::string(charArray);
    delete[] charArray;
    return string;
}

extern std::string projectPath;

std::string fullPath(std::string localPath)
{
    return projectPath + localPath;
}

float clamp(float value, float min, float max)
{
    if (value > max)
    {
        return max;
    }
    else if (value < min)
    {
        return min;
    }

    return value;
}

float remap(float value, float inStart, float inEnd, float outStart, float outEnd)
{
    value = outStart + (value - inStart) * (outEnd - outStart) / (inEnd - inStart);

    if (outStart < outEnd)
    {
        value = clamp(value, outStart, outEnd);
    }
    else
    {
        value = clamp(value, outEnd, outStart);
    }

    return value;
}

bool inRange(float value, float min, float max)
{
    return value >= min && value <= max;
}
