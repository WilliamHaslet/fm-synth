#include "glfwCallbacks.hpp"
#include "glfw.hpp"
#include <iostream>

static void errorCallback(int error, const char* description)
{
    std::cout << "ERROR: glfw description: " << description << std::endl;
}

GLFWCallbacks::GLFWCallbacks(Input* input, Window* window) : input(input), window(window)
{
    glfwSetErrorCallback(errorCallback);
    
    glfwSetWindowUserPointer(window->getWindow(), this);
    
    auto framebaufferCallbackTemp = [](GLFWwindow* window, int width, int height)
    {
        GLFWCallbacks* c = (GLFWCallbacks*)glfwGetWindowUserPointer(window);
        c->window->framebufferSizeCallback(window, width, height);
    };
    
    auto windowSizeCallbackTemp = [](GLFWwindow* window, int width, int height)
    {
        GLFWCallbacks* c = (GLFWCallbacks*)glfwGetWindowUserPointer(window);
        c->window->windowSizeCallback(window, width, height);
    };
    
    glfwSetFramebufferSizeCallback(window->getWindow(), framebaufferCallbackTemp);
    glfwSetWindowSizeCallback(window->getWindow(), windowSizeCallbackTemp);
}
