#pragma once
#include "glfw.hpp"
#include <string>

class Window
{
public:
    Window();
    void display();
    bool running();
    void close();
    void setTitle(std::string title);
    int getFramebufferWidth();
    int getFramebufferHeight();
    float getAspectRatio();
    GLFWwindow* getWindow();
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void windowSizeCallback(GLFWwindow* window, int width, int height);
    
private:
    int windowWidth = 800;
    int windowHeight = 600;
    int framebufferWidth = 800;
    int framebufferHeight = 600;
    std::string windowTitle = "OpenGL Window";
    GLFWwindow* window;
};
