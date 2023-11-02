#pragma once
#include <chrono>

class Time
{
public:
    Time();
    void update();
    float getTime();
    int getFrames();
    float getFrameTime();
    float getFrameRate();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    int frames = 0;
    double time = 0;
    double frameTime = 0;
    double frameRate = 0;
};
