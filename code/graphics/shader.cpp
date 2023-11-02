#include "shader.hpp"
#include "debug.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(std::string shaderFile) : shaderName(shaderFile)
{
    std::string vertSource;
    std::string fragSource;
    processFile(shaderFile, vertSource, fragSource);
    linkShader(vertSource, fragSource);
}

Shader::Shader(std::string vertFile, std::string fragFile) : shaderName(vertFile + fragFile)
{
    linkShader(readFile(vertFile), readFile(fragFile));
}

Shader::~Shader()
{
    glUseProgram(0);
    glDeleteProgram(program);
}

void Shader::setTextureUniforms(std::initializer_list<std::string> textureUniforms)
{
    const std::string* first = textureUniforms.begin();
    glUseProgram(program);
    
    for (int i = 0; i < textureUniforms.size(); i++)
    {
        std::string name = first[i];
        int uniform = glGetUniformLocation(program, name.c_str());
        
        if (uniform < 0)
        {
            std::cout << "ERROR: texture uniform " << name << " not found: " << shaderName << std::endl;
        }
        else
        {
            glUniform1i(uniform, i);
        }
    }
    
    glUseProgram(0);
}

void Shader::processFile(std::string shaderFile, std::string& vertSource, std::string& fragSource)
{
    std::ifstream fileStream(shaderFile);

    if (!fileStream)
    {
        std::cout << "ERROR: shader file not found: " << shaderFile << std::endl;
        exit(1);
    }
    
    std::string line;
    int output = -1;
    
    while (std::getline(fileStream, line))
    {
        if (line.find("#shader shared") != std::string::npos)
        {
            output = 0;
        }
        else if (line.find("#shader vertex") != std::string::npos)
        {
            output = 1;
        }
        else if (line.find("#shader fragment") != std::string::npos)
        {
            output = 2;
        }
        else
        {
            line += "\n";
            
            if (output == 0)
            {
                vertSource += line;
                fragSource += line;
            }
            else if (output == 1)
            {
                vertSource += line;
            }
            else if (output == 2)
            {
                fragSource += line;
            }
        }
    }
}

void Shader::linkShader(std::string vertSource, std::string fragSource)
{
    GLuint vertexShader = compileShader(vertSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragSource, GL_FRAGMENT_SHADER);
    
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    //glBindFragDataLocation(program, 0, "outColor"); // Not needed with only one output because 0 is the default
    glLinkProgram(program);
    
    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    GLint logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    
    if (logLength > 0)
    {
        char* infoLog = new char[logLength];
        glGetProgramInfoLog(program, logLength, NULL, infoLog);
        
        if (success != GL_TRUE) // Error
        {
            std::cout << "ERROR: shader link: " << shaderName << "\n" << infoLog << std::endl;
            exit(1);
        }
        else // Warning
        {
            std::cout << "WARNING: shader link: " << shaderName << "\n" << infoLog << std::endl;
        }

        delete[] infoLog;
    }
    
    // Once program is linked, detach and delete shaders to free up memory
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

std::string Shader::readFile(std::string filePath)
{
    std::ifstream fileStream(filePath);

    if (!fileStream)
    {
        std::cout << "ERROR: shader file not found: " << filePath << std::endl;
        exit(1);
    }

    std::ostringstream stringStream;
    stringStream << fileStream.rdbuf();
    fileStream.close();
    
    return stringStream.str();
}

GLuint Shader::compileShader(std::string sourceString, GLenum type)
{
    GLuint shader;
    shader = glCreateShader(type);
    const char* source = sourceString.c_str();
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    
    if (logLength > 0)
    {
        char* infoLog = new char[logLength];
        glGetShaderInfoLog(shader, logLength, NULL, infoLog);
        
        std::string typeName = "unknown type";
        
        if (type == GL_VERTEX_SHADER)
        {
            typeName = "vertex";
        }
        else if (type == GL_FRAGMENT_SHADER)
        {
            typeName = "fragment";
        }
        
        if (success != GL_TRUE) // Error
        {
            std::cout << "ERROR: " + typeName + " shader compile: " << shaderName << "\n" << infoLog << std::endl;
            exit(1);
        }
        else // Warning
        {
            std::cout << "WARNING: " + typeName + " shader compile: " << shaderName << "\n" << infoLog << std::endl;
        }

        delete[] infoLog;
    }
    
    return shader;
}

void Shader::validateShaderProgram()
{
    glValidateProgram(program);
    
    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    GLint logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    
    if (logLength > 0)
    {
        char* infoLog = new char[logLength];
        glGetProgramInfoLog(program, logLength, NULL, infoLog);
        
        if (success != GL_TRUE) // Error
        {
            std::cout << "ERROR: shader validation: " << shaderName << "\n" << infoLog << std::endl;
        }
        else // Warning
        {
            std::cout << "MESSAGE: shader validation: " << shaderName << "\n" << infoLog << std::endl;
        }

        delete[] infoLog;
    }
}
