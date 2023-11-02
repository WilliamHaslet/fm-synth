#include "input.hpp"
#include "glfw.hpp"
#include <glm/glm.hpp>

Input::Input(Window* window) : window(window)
{

}

void Input::handleInput()
{
    if (escapeReleased && glfwGetKey(window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        escapeReleased = false;
        glfwSetWindowShouldClose(window->getWindow(), true);
    }
    
    if (glfwGetKey(window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_RELEASE)
    {
        escapeReleased = true;
    }

    if (mouse1Released && mouseHeld())
    {
        mouse1Down = true;
        mouse1Released = false;
    }
    else
    {
        mouse1Down = false;
    }

    if (!mouse1Released && !mouseHeld())
    {
        mouse1Up = true;
        mouse1Released = true;
    }
    else
    {
        mouse1Up = false;
    }
    
    setMousePosition();
}

void Input::setMousePosition()
{
    double x;
    double y;
    glfwGetCursorPos(window->getWindow(), &x, &y);
    
    mouseDelta.x = x - mousePosition.x;
    mouseDelta.y = ((window->getFramebufferHeight() - y) - mousePosition.y);
    mousePosition.x = x;
    mousePosition.y = window->getFramebufferHeight() - y;
    
    if (initalizeMousePosition)
    {
        initalizeMousePosition = false;
        mouseDelta.x = 0;
        mouseDelta.y = 0;
    }
}

Vec2 Input::getMouseDelta()
{
    return mouseDelta;
}

Vec2 Input::getMousePosition()
{
    return mousePosition;
}

bool Input::mouseHeld()
{
    return glfwGetMouseButton(window->getWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
}

bool Input::mouseDown()
{
    return mouse1Down;
}

bool Input::mouseUp()
{
    return mouse1Up;
}
