#include "gui.hpp"
#include "glfwCallbacks.hpp"
#include "debug.hpp"
#include "../utility.hpp"
#include "knob.hpp"
#include <vector>

void GUI::start(Controls& controls)
{
    time = new Time();
    window = new Window();
    input = new Input(window);

    GLFWCallbacks callbacks(input, window);

    openglErrorCheck();

    Vec2 screenSize = Vec2(window->getFramebufferWidth(), window->getFramebufferHeight());
    
    std::vector<std::string> names = { "attack", "decay", "sustain", "release" };
    std::vector<Knob*> knobs;
    Knob* activeKnob = NULL;
    Vec2 pos = Vec2(100, 100);

    for (int i = 0; i < 4; i++)
    {
        knobs.push_back(new Knob(screenSize, time, controls.getControl(names[i])));
        knobs[i]->setPosition(pos);
        pos.x += 120;
    }

    Rectangle cursor = Rectangle(glm::vec3(0, 1, 1), screenSize);
    cursor.size = Vec2(10, 10);
    cursor.layer = 1;

    while (window->running())
    {
        time->update();
        input->handleInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < knobs.size(); i++)
        {
            if (input->mouseDown() && knobs[i]->pointInBounds(input->getMousePosition()))
            {
                activeKnob = knobs[i];
            }
        }

        if (input->mouseUp())
        {
            activeKnob = NULL;
        }

        if (activeKnob != NULL && input->getMouseDelta().y != 0)
        {
            activeKnob->turn(input->getMouseDelta().y);
        }

        for (int i = 0; i < knobs.size(); i++)
        {
            knobs[i]->draw();
        }

        cursor.position = input->getMousePosition() + Vec2(cursor.size.x / 2, -cursor.size.y / 2);
        cursor.draw();

        openglErrorCheck();

        //windowTitleInfo(window, time);
        fpsCounter(window);

        window->display();
    }

    window->close();

    delete input;
    delete time;
    delete window;
}
