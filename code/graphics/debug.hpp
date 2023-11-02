#pragma once
#include "window.hpp"
#include "time.hpp"
#include <string>
#include "gl.hpp"

std::string openglVersion();
std::string openglRenderingHardware();
void openglErrorCheck();
void currentVertexBuffer();
void currentElementBuffer();
void windowTitleInfo(Window window, Time time);
void fpsCounter(Window* window);
std::string currentWorkingDirectory();
int maxVertexAttributes();
std::string openglInfo();
void printShaderUniforms(GLuint shaderProgram);
