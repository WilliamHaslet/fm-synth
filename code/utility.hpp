#pragma once
#include <string>

std::string roundedFloatString(float number, int decimalPlaces, int maxLength = 10);
std::string fullPath(std::string localPath);

static const double PI = 3.14159265358979323846;
// const float pi = 3.14159265f;

float clamp(float value, float min, float max);
float remap(float value, float inStart, float inEnd, float outStart, float outEnd);
bool inRange(float value, float min, float max);
