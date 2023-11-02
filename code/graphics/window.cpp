#include "window.hpp"
#include "gl.hpp"
#include <iostream>

Window::Window()
{
    if (!glfwInit())
    {
        std::cout << "ERROR: glfw init error" << std::endl;
        exit(1);
    }
    
    //glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    bool fullscreen = false;
    
    if (fullscreen)
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vmode = glfwGetVideoMode(monitor);
        window = glfwCreateWindow(vmode->width, vmode->height, windowTitle.c_str(), monitor, NULL);
    }
    else
    {
        window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
    }
    
    if (!window)
    {
        glfwTerminate();
        std::cout << "ERROR: glfw window error" << std::endl;
        exit(1);
    }
    
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK)
    {
        std::cout << "ERROR: glew init" << std::endl;
    }
    
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight); // Use framebuffer size for correct viewport scaling on retina displays
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    //glfwSwapInterval(1); // v-sync?
    //glfwSwapInterval(0); // Force v-sync off. It's on by default
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    framebufferWidth = width;
    framebufferHeight = height;
    glViewport(0, 0, framebufferWidth, framebufferHeight);
}

void Window::windowSizeCallback(GLFWwindow* window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

bool Window::running()
{
    return !glfwWindowShouldClose(window);
}

void Window::display()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::close()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::setTitle(std::string title)
{
    glfwSetWindowTitle(window, title.c_str());
}

int Window::getFramebufferWidth()
{
    return framebufferWidth;
}

int Window::getFramebufferHeight()
{
    return framebufferHeight;
}

float Window::getAspectRatio()
{
    return framebufferWidth / (float)framebufferHeight;
}

GLFWwindow* Window::getWindow()
{
    return window;
}
