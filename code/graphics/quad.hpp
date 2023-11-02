#pragma once
#include "shader.hpp"
#include "textureSet.hpp"
#include "gl.hpp"

class Quad
{
private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    Shader shader;
    TextureSet* textureSet = NULL;
    
public:
    Quad(Shader& shader);
    Quad(Shader& shader, TextureSet* textureSet);
    ~Quad();
    void draw();

private:
    void setup();
};
