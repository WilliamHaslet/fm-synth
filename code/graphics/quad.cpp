#include "quad.hpp"

Quad::Quad(Shader& shader) : shader(shader)
{
    setup();
}

Quad::Quad(Shader& shader, TextureSet* textureSet) : shader(shader), textureSet(textureSet)
{
    setup();
}

Quad::~Quad()
{
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ebo);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
}

void Quad::setup()
{
    float vertices[] = { // vec2 position and vec2 uvs
        0, 0, 0, 0,
        1, 0, 1, 0,
        0, 1, 0, 1,
        1, 1, 1, 1
    };
    
    GLuint indices[] = {
        0, 1, 3,
        0, 3, 2
    };
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Verts
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // UVs
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindVertexArray(0); // Unbind vao for later use

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vbo, now stored in vao with attrib pointer calls (or enable vertex attrib array calls?)
    // (not needed, unbinding vao unbinds ebo) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind ebo, now stored in vao
}

void Quad::draw()
{
    glUseProgram(shader.program);
    glBindVertexArray(vao);
    
    glEnable(GL_CULL_FACE); // Enable culling
    //glCullFace(GL_FRONT); // Cull only front face
    glCullFace(GL_BACK); // Cull only back face
    //glFrontFace(GL_CW); // Set clockwise to be the front face
    //glFrontFace(GL_CCW); // Or set counter clockwise to be the front face (default)
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Render in wireframe mode
    
    //shader.validateShaderProgram(); // Check shader validity in current OpenGL state machine context
    
    if (textureSet != NULL)
    {
        textureSet->bindTextures();
    }
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Disable wireframe mode
    
    glDisable(GL_CULL_FACE); // Disable culling

    glBindVertexArray(0);
    glUseProgram(0);
}
