#include "time.hpp"
#include "glfw.hpp"

Time::Time()
{
    startTime = std::chrono::high_resolution_clock::now();
}

void Time::update()
{
    auto current = std::chrono::high_resolution_clock::now();
    double lastTime = time;
    time = std::chrono::duration_cast<std::chrono::duration<double>>(current - startTime).count();
    frameTime = time - lastTime;
    frameRate = 1 / frameTime;
    frames++;
}

float Time::getTime()
{
    return time;
}

int Time::getFrames()
{
    return frames;
}

float Time::getFrameTime()
{
    return frameTime;
}

float Time::getFrameRate()
{
    return frameRate;
}
