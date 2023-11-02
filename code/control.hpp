#pragma once
#include <vector>
#include <functional>

class Control
{
private:
	float value;
	std::vector<std::function<void()>> changeCallbacks;

public:
	float getValue();
	void setValue(float newValue);
	void changeEvent();
	void addChangeCallback(std::function<void()> callback);
};
