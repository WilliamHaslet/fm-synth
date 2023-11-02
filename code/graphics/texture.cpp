#include "texture.hpp"
#include "../utility.hpp"
#include <stb_image.h>
#include <iostream>

Texture::Texture(std::string filePath, GLenum format, bool repeat)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    
    if (!data)
    {
        std::cout << "ERROR: image failed to load: " << filePath << std::endl;
        exit(1);
    }
    
    id = createTexture(data, GL_UNSIGNED_BYTE, width, height, format, false, false, true);
    
    stbi_image_free(data); // Done with original data now
}

Texture::Texture(float* data, int width, int height, GLenum format, bool repeat)
{
    id = createTexture(data, GL_FLOAT, width, height, format, false, true, repeat);
}

Texture::~Texture()
{
    glBindTexture(GL_TEXTURE_2D, 0); // Is this needed or correct?
    glDeleteTextures(1, &id);
}

GLuint Texture::createTexture(void* data, GLenum type, int width, int height, GLenum format, bool mipmaps, bool pointFilter, bool repeat)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
    
    if (mipmaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    // GL_REPEAT (default)
    // GL_MIRRORED_REPEAT
    // GL_CLAMP_TO_EDGE
    // GL_CLAMP_TO_BORDER (need to specify border color)
    
    if (repeat)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    
    /* // For GL_CLAMP_TO_BORDER
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    */
    
    if (pointFilter)
    {
        if (mipmaps) // Mipmaps only used for downscaling
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Scaling up
    }
    else
    {
        if (mipmaps) // Mipmaps only used for downscaling
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Scaling up
    }
    
    return texture;
}
