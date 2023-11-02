#include "control.hpp"

float Control::getValue()
{
	return value;
}

void Control::setValue(float newValue)
{
	value = newValue;
}

void Control::changeEvent()
{
	for (int i = 0; i < changeCallbacks.size(); i++)
	{
		changeCallbacks[i]();
	}
}

void Control::addChangeCallback(std::function<void()> callback)
{
	changeCallbacks.push_back(callback);
}
