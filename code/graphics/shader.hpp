#pragma once
#include "gl.hpp"
#include <string>

class Shader 
{
public:
    GLuint program;
    
private:
    std::string shaderName;
    
public:
    Shader(std::string shaderFile);
    Shader(std::string vertFile, std::string fragFile);
    ~Shader();
    void validateShaderProgram();
    void setTextureUniforms(std::initializer_list<std::string> textureUniforms);
    
private:
    GLuint compileShader(std::string sourceString, GLenum type);
    std::string readFile(std::string filePath);
    void linkShader(std::string vertSource, std::string fragSource);
    void processFile(std::string shaderFile, std::string& vertSource, std::string& fragSource);
};
