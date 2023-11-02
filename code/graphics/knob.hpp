#pragma once
#include "Vec2.hpp"
#include "gl.hpp"
#include "quad.hpp"
#include "../utility.hpp"
#include "time.hpp"
#include "../control.hpp"
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Rectangle
{
public:
    glm::vec3 color = glm::vec3(1, 1, 1);
    Vec2 position = Vec2(0, 0);
    float rotation = 0;
    Vec2 size = Vec2(100, 100);
    Vec2 screenSize = Vec2(800, 600);
    float layer = 0;

private:
    Shader* shader = 0;
    Quad* quad = 0;
    GLint colorUniform = -1;
    GLint modelMatrixUniform = -1;
    GLint viewMatrixUniform = -1;
    GLint projectionMatrixUniform = -1;

public:
    Rectangle()
    {

    }

    Rectangle(glm::vec3 color, Vec2 screenSize) : color(color), screenSize(screenSize)
    {
        shader = new Shader(fullPath("shaders/guiColor.glsl"));
        quad = new Quad(*shader);

        colorUniform = glGetUniformLocation(shader->program, "color");
        modelMatrixUniform = glGetUniformLocation(shader->program, "modelMatrix");
        viewMatrixUniform = glGetUniformLocation(shader->program, "viewMatrix");
        projectionMatrixUniform = glGetUniformLocation(shader->program, "projectionMatrix");

        glm::mat4 viewMatrix = glm::translate(glm::vec3(0, 0, -10));
        glm::mat4 projectionMatrix = glm::ortho(0.0f, screenSize.x, 0.0f, screenSize.y, 0.1f, 100.0f);

        glUseProgram(shader->program);
        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUseProgram(0);
    }

    void draw()
    {
        glm::mat4 translate = glm::translate(glm::vec3(position.x, position.y, layer));
        glm::mat4 rotate = glm::rotate(glm::radians(rotation), glm::vec3(0, 0, 1));
        glm::mat4 scale = glm::scale(glm::vec3(size.x, size.y, 1));
        glm::mat4 modelMatrix = translate * rotate * scale;

        glUseProgram(shader->program);
        glUniform3f(colorUniform, color.x, color.y, color.z);
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUseProgram(0);

        quad->draw();
    }

    Vec2 up()
    {
        return Vec2(cos(glm::radians(rotation + 90)), sin(glm::radians(rotation + 90)));
    }
};

#include <iostream>

class Knob
{
private:
    Rectangle background;
    Rectangle foreground;
    Rectangle tip;
    Time* time = 0;
    Control* control = 0;
    const float maxAngle = 150;

public:
    Knob(Vec2 screenSize, Time* time, Control* control) : time(time), control(control)
    {
        background = Rectangle(glm::vec3(0.2, 0.2, 0.2), screenSize);
        foreground = Rectangle(glm::vec3(1, 1, 1), screenSize);
        tip = Rectangle(glm::vec3(1, 0, 0), screenSize);

        control->addChangeCallback([this]() {
            foreground.rotation = valueToRotation(this->control->getValue());
        });
    }

    void draw()
    {
        tip.position = foreground.position + (foreground.up() * (foreground.size.y / 2));
        tip.rotation = foreground.rotation;

        background.draw();
        foreground.draw();
        tip.draw();
    }

    float rotationToValue(float rotation)
    {
        return remap(rotation, maxAngle, -maxAngle, 0, 1);
    }

    float valueToRotation(float value)
    {
        return remap(value, 0, 1, maxAngle, -maxAngle);
    }

    void turn(float mouseDelta)
    {
        float turnSpeed = 1.5f;
        foreground.rotation -= mouseDelta * turnSpeed;

        if (foreground.rotation > maxAngle)
        {
            foreground.rotation = maxAngle;
        }
        else if (foreground.rotation < -maxAngle)
        {
            foreground.rotation = -maxAngle;
        }

        control->setValue(rotationToValue(foreground.rotation));
        control->changeEvent();
    }

    bool pointInBounds(Vec2 point)
    {
        float left = background.position.x - (background.size.x / 2);
        float right = background.position.x + (background.size.x / 2);
        float bottom = background.position.y - (background.size.y / 2);
        float top = background.position.y + (background.size.y / 2);
        return point.x >= left && point.x <= right && point.y >= bottom && point.y <= top;
    }

    void setPosition(Vec2 position)
    {
        setupVisuals(position);
    }

private:
    void setupVisuals(Vec2 position)
    {
        background.position = position + Vec2(100, 100);

        foreground.size = background.size / 2;
        foreground.position = background.position;
        foreground.layer = 0.1;
        foreground.rotation = valueToRotation(control->getValue());

        tip.size = Vec2(10, 10);
        tip.layer = 0.2;
    }
};
