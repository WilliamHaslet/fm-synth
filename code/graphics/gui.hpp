#pragma once
#include "input.hpp"
#include "time.hpp"
#include "window.hpp"
#include "../controls.hpp"

class GUI
{
private:
	Input* input;
	Time* time;
	Window* window;

public:
	void start(Controls& controls);
};
