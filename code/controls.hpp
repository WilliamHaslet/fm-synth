#pragma once
#include "control.hpp"
#include <unordered_map>
#include <string>

class Controls
{
private:
	std::unordered_map<std::string, Control*> controls;

public:
	Control* newControl(std::string name);
	Control* getControl(std::string name);
};
