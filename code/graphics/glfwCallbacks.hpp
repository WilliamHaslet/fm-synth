#pragma once
#include "input.hpp"
#include "window.hpp"

class GLFWCallbacks
{
private:
    Input* input;
    Window* window;
    
public:
    GLFWCallbacks(Input* input, Window* window);
};
