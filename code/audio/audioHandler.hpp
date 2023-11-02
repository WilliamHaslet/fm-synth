#pragma once
#include "RtAudio.h"
#include "shared.hpp"

class AudioHandler
{
public:
	SharedData* sharedData = 0;

private:
	float maxLevel = 0.002;
	float level = 0.002;
	bool asio = false;
	const unsigned int sampleRate = 44100;
	RtAudio* rtAudio = 0;

public:
	AudioHandler();
	~AudioHandler();
	int handleAudio(float* outputBuffer, float* inputBuffer, int frameCount, double streamTime, RtAudioStreamStatus status);

private:
	float earProtection(float sample);
	void printDevice(RtAudio::DeviceInfo info);
	void printDevice(unsigned int deviceId);
	void printDevices(bool inputs, bool outputs);
	int getDevice(RtAudio* rtAudio, std::string name);
	void handleCC(SharedData* sharedData, int cc, int value);
};
