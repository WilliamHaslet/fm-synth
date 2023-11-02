#include "controls.hpp"
#include <iostream>

Control* Controls::newControl(std::string name)
{
	if (controls[name] != NULL)
	{
		std::cout << "Control '" << name << "' already exists" << std::endl;
		exit(1);
	}

	controls[name] = new Control();
	return controls[name];
}

Control* Controls::getControl(std::string name)
{
	Control* control = controls[name];

	if (control == NULL)
	{
		std::cout << "Control '" << name << "' does not exist" << std::endl;
		exit(1);
	}

	return control;
}
