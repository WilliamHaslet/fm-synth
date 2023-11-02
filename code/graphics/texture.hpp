#pragma once
#include <string>
#include "gl.hpp"

class Texture
{
public:
    GLuint id;
    
public:
    Texture(std::string localPath, GLenum format, bool repeat);
    Texture(float* data, int width, int height, GLenum format, bool repeat);
    ~Texture();
    
private:
    GLuint createTexture(void* data, GLenum type, int width, int height, GLenum format, bool mipmaps, bool pointFilter, bool repeat);
};
