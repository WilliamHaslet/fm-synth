#include "debug.hpp"
#include "..\utility.hpp"
#include <iostream>
#include <cstdio>
#include <sstream>

std::string openglVersion()
{
    //return std::string((char*)glGetString(GL_VERSION));
    int version[2];
    glGetIntegerv(GL_MAJOR_VERSION, &version[0]);
    glGetIntegerv(GL_MINOR_VERSION, &version[1]);
    return std::to_string(version[0]) + "." + std::to_string(version[1]);
}

std::string openglRenderingHardware()
{
    return std::string((char*)glGetString(GL_RENDERER));
}

void openglErrorCheck()
{
    GLenum errorCheckValue = glGetError();
    
    if (errorCheckValue != GL_NO_ERROR)
    {
        std::cout << "ERROR: OpenGL Code: " << errorCheckValue << std::endl;
        exit(1);
    }
}

void currentVertexBuffer()
{
    GLint id;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id); // Get current vbo
    std::cout << id << std::endl;
}

void currentElementBuffer()
{
    GLint id;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &id); // Get current ebo
    std::cout << id << std::endl;
}

void windowTitleInfo(Window window, Time time)
{
    std::string fps = roundedFloatString(time.getFrameRate(), 0);
    std::string frameTime = roundedFloatString(time.getFrameTime(), 4);
    //std::string frames = std::to_string(time.getFrames());
    std::string timeString = roundedFloatString(time.getTime(), 1);
    std::string spacer = "\t";
    std::string title =
        "FPS: " + fps + spacer +
        //"F: " + frames + spacer +
        "FT: " + frameTime + spacer +
        "T: " + timeString + spacer;
    window.setTitle(title); 
}

void fpsCounter(Window* window)
{
    static double previous_seconds = glfwGetTime();
    static int frame_count;
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25)
    {
        previous_seconds = current_seconds;
        double fps = (double)frame_count / elapsed_seconds;
        char tmp[128];
        snprintf(tmp, 128, "opengl @ fps: %.2f", fps);
        window->setTitle(tmp);
        frame_count = 0;
    }
    frame_count++;
}

int maxVertexAttributes()
{
    int v;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &v);
    return v;
}

std::string openglInfo()
{
    std::stringstream stream;
    GLenum params[] = {
        GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
        GL_MAX_CUBE_MAP_TEXTURE_SIZE,
        GL_MAX_DRAW_BUFFERS,
        GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
        GL_MAX_TEXTURE_IMAGE_UNITS,
        GL_MAX_TEXTURE_SIZE,
        //GL_MAX_VARYING_FLOATS, // Gives OpenGL invalid enum error
        GL_MAX_VERTEX_ATTRIBS,
        GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
        GL_MAX_VERTEX_UNIFORM_COMPONENTS,
        GL_MAX_VIEWPORT_DIMS,
        GL_STEREO,
    };
    const char* names[] = {
        "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
        "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
        "GL_MAX_DRAW_BUFFERS",
        "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
        "GL_MAX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_TEXTURE_SIZE",
        //"GL_MAX_VARYING_FLOATS",
        "GL_MAX_VERTEX_ATTRIBS",
        "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
        "GL_MAX_VIEWPORT_DIMS",
        "GL_STEREO",
    };
    stream << "GL Context Params:\n";
    
    int count = 9;
    for (int i = 0; i < count; i++)
    {
        int v = 0;
        glGetIntegerv(params[i], &v);
        stream << names[i] << " " << v << "\n";
    }
    // others
    int v[2];
    v[0] = v[1] = 0;
    glGetIntegerv(params[count], v);
    stream << names[count] << " " << v[0] << " " << v[1] << "\n";
    unsigned char s = 0;
    glGetBooleanv(params[count + 1], &s);
    stream << names[count + 1] << " " << (unsigned int)s << "\n";
    return stream.str();
}

const char* GL_type_to_string(GLenum type)
{
    switch (type)
    {
        case GL_BOOL: return "bool";
        case GL_INT: return "int";
        case GL_FLOAT: return "float";
        case GL_FLOAT_VEC2: return "vec2";
        case GL_FLOAT_VEC3: return "vec3";
        case GL_FLOAT_VEC4: return "vec4";
        case GL_FLOAT_MAT2: return "mat2";
        case GL_FLOAT_MAT3: return "mat3";
        case GL_FLOAT_MAT4: return "mat4";
        case GL_SAMPLER_2D: return "sampler2D";
        case GL_SAMPLER_3D: return "sampler3D";
        case GL_SAMPLER_CUBE: return "samplerCube";
        case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
        default: break;
    }
    return "other";
}

void printShaderUniforms(GLuint shaderProgram)
{
    printf("--------------------\nshader programme %i info:\n", shaderProgram);
    int params = -1;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &params);
    printf("GL_LINK_STATUS = %i\n", params);

    glGetProgramiv(shaderProgram, GL_ATTACHED_SHADERS, &params);
    printf("GL_ATTACHED_SHADERS = %i\n", params);

    glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTES, &params);
    printf("GL_ACTIVE_ATTRIBUTES = %i\n", params);
    for (int i = 0; i < params; i++)
    {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveAttrib(shaderProgram, i, max_length, &actual_length, &size, &type, name);
        if (size > 1)
        {
            for (int j = 0; j < size; j++)
            {
                char long_name[64];
                snprintf(long_name, 64, "%s[%i]", name, j);
                int location = glGetAttribLocation(shaderProgram, long_name);
                printf("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), long_name, location);
            }
        }
        else
        {
            int location = glGetAttribLocation(shaderProgram, name);
            printf("  %i) type:%s name:%s location:%i\n",
            i, GL_type_to_string(type), name, location);
        }
    }

    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &params);
    printf("GL_ACTIVE_UNIFORMS = %i\n", params);
    for (int i = 0; i < params; i++)
    {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveUniform(shaderProgram, i, max_length, &actual_length, &size, &type, name);
        if (size > 1)
        {
            for (int j = 0; j < size; j++)
            {
                char long_name[64];
                snprintf(long_name, 64, "%s[%i]", name, j);
                int location = glGetUniformLocation(shaderProgram, long_name);
                printf("  %i) type:%s name:%s location:%i\n",
                i, GL_type_to_string(type), long_name, location);
            }
        }
        else
        {
            int location = glGetUniformLocation(shaderProgram, name);
            printf("  %i) type:%s name:%s location:%i\n",
            i, GL_type_to_string(type), name, location);
        }
    }
}
