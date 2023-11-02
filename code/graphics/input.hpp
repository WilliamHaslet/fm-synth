#pragma once
#include "window.hpp"
#include "vec2.hpp"

class Input
{
private:
    Vec2 mouseDelta = Vec2(0, 0);
    Vec2 mousePosition = Vec2(0, 0);
    Vec2 moveInput = Vec2(0, 0);
    Window* window;
    bool initalizeMousePosition = true;
    bool escapeReleased = true;

    bool mouse1Released = true;
    bool mouse1Down = false;
    bool mouse1Up = false;
    
public:
    Input(Window* window);
    void handleInput();
    Vec2 getMouseDelta();
    Vec2 getMousePosition();

    bool mouseDown();
    bool mouseUp();
    bool mouseHeld();
    
private:
    void setMousePosition();
};
