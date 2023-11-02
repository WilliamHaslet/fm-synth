#include "audioHandler.hpp"

float AudioHandler::earProtection(float sample)
{
    if (sample > maxLevel)
    {
        std::cout << "Warning: over max level" << std::endl;
        sample = maxLevel;
    }
    else if (sample < -maxLevel)
    {
        std::cout << "Warning: over max level" << std::endl;
        sample = -maxLevel;
    }

    return sample;
}

void AudioHandler::printDevice(RtAudio::DeviceInfo info)
{
    std::cout << "ID:          " << info.ID << std::endl;
    std::cout << "Name:        " << info.name << std::endl;
    std::cout << "Sample Rate: " << info.preferredSampleRate << std::endl;
    std::cout << "In:          " << info.inputChannels << std::endl;
    std::cout << "Out:         " << info.outputChannels << std::endl;
    std::cout << "Duplex:      " << info.duplexChannels << std::endl;
}

void AudioHandler::printDevice(unsigned int deviceId)
{
    printDevice(rtAudio->getDeviceInfo(deviceId));
}

void AudioHandler::printDevices(bool inputs, bool outputs)
{
    std::vector<unsigned int> deviceIds = rtAudio->getDeviceIds();

    for (int i = 0; i < deviceIds.size(); i++)
    {
        RtAudio::DeviceInfo info = rtAudio->getDeviceInfo(deviceIds[i]);

        if (inputs && info.inputChannels > 0)
        {
            std::cout << "------------" << std::endl;
            printDevice(info);
        }
        else if (outputs && info.outputChannels > 0)
        {
            std::cout << "------------" << std::endl;
            printDevice(info);
        }
    }

    std::cout << "\n------default------" << std::endl;
    printDevice(rtAudio->getDefaultOutputDevice());
}

int AudioHandler::getDevice(RtAudio* rtAudio, std::string name)
{
    std::vector<unsigned int> deviceIds = rtAudio->getDeviceIds();

    for (int i = 0; i < deviceIds.size(); i++)
    {
        RtAudio::DeviceInfo info = rtAudio->getDeviceInfo(deviceIds[i]);

        if (info.name == name)
        {
            return deviceIds[i];
        }
    }

    return -1;
}

void AudioHandler::handleCC(SharedData* sharedData, int cc, int value)
{
    float val = value / 127.0f;

    if (cc == 16)
    {
        sharedData->generator->envelope->attackControl->setValue(val);
        sharedData->generator->envelope->attackControl->changeEvent();
    }
    else if (cc == 17)
    {
        sharedData->generator->envelope->decayControl->setValue(val);
        sharedData->generator->envelope->decayControl->changeEvent();
    }
    else if (cc == 18)
    {
        sharedData->generator->envelope->sustainControl->setValue(val);
        sharedData->generator->envelope->sustainControl->changeEvent();
    }
    else if (cc == 19)
    {
        sharedData->generator->envelope->releaseControl->setValue(val);
        sharedData->generator->envelope->releaseControl->changeEvent();
    }
}

int AudioHandler::handleAudio(float* outputBuffer, float* inputBuffer, int frameCount, double streamTime, RtAudioStreamStatus status)
{
    if (status)
    {
        std::cout << "Stream underflow detected!" << std::endl;
    }

    while (sharedData->messages.size() > 0)
    {
        MidiMessage message = sharedData->messages.front();
        sharedData->messages.pop();

        if (message.type == MidiMessageType::NoteOn)
        {
            ActiveNote note;
            note.note = message.note;
            note.velocity = message.velocity;
            sharedData->activeNotes.push_back(note);
        }
        else if (message.type == MidiMessageType::NoteOff)
        {
            for (int i = 0; i < sharedData->activeNotes.size(); i++)
            {
                if (sharedData->activeNotes[i].note == message.note)
                {
                    sharedData->activeNotes[i].released = true;
                }
            }
        }
        else if (message.type == MidiMessageType::CC)
        {
            handleCC(sharedData, message.cc, message.value);
        }
    }

    for (int j = 0; j < frameCount; j++)
    {
        float time = sharedData->frames / (float)sampleRate;
        float x = 0;

        // Collect signal data from all active notes
        for (int i = 0; i < sharedData->activeNotes.size(); i++)
        {
            x += sharedData->generator->evaluate(sharedData->activeNotes[i], time);

            if (sharedData->activeNotes[i].released)
            {
                sharedData->activeNotes[i].endTimer += 1.0f / sampleRate;

                // Note has finished, remove it
                if (sharedData->activeNotes[i].done)
                {
                    sharedData->activeNotes.erase(sharedData->activeNotes.begin() + i);
                    i--;
                }
            }
            else
            {
                sharedData->activeNotes[i].startTimer += 1.0f / sampleRate;
            }
        }

        *outputBuffer = earProtection(x * level);
        outputBuffer++;
        *outputBuffer = earProtection(x * level);
        outputBuffer++;

        sharedData->frames++;
    }

    return 0;
}

AudioHandler::AudioHandler()
{
    int deviceId = -1;
    std::string apiName;

    if (asio)
    {
        rtAudio = new RtAudio(RtAudio::Api::WINDOWS_ASIO);
        deviceId = getDevice(rtAudio, "Studio USB ASIO Driver");
        level = 0.002;
        maxLevel = 0.002;
        apiName = rtAudio->getApiDisplayName(RtAudio::Api::WINDOWS_ASIO);
    }
    else
    {
        rtAudio = new RtAudio(RtAudio::Api::WINDOWS_WASAPI);
        deviceId = getDevice(rtAudio, "Main Out 1/2 (3- Studio 24c)");
        //deviceId = rtAudio->getDefaultOutputDevice();
        level = 0.1;
        maxLevel = 0.8;
        apiName = rtAudio->getApiDisplayName(RtAudio::Api::WINDOWS_WASAPI);
    }

    if (deviceId < 0)
    {
        std::cout << "Error: device not found" << std::endl;
        exit(1);
    }

    //printDevices(rtAudio, false, true);

    std::cout << "------------------------------- " << std::endl;
    std::cout << "Api:         " << apiName << std::endl;
    std::cout << "Device:      " << rtAudio->getDeviceInfo(deviceId).name << std::endl;
    std::cout << "Sample Rate: " << rtAudio->getDeviceInfo(deviceId).preferredSampleRate << std::endl;
    std::cout << "------------------------------- " << std::endl;

    RtAudio::StreamParameters parameters;
    parameters.deviceId = deviceId;
    parameters.nChannels = 2;
    parameters.firstChannel = 0;

    unsigned int bufferFrames = 256; // 256 sample frames

    auto audioCallback = [](void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void* userPointer)
        {
            return ((AudioHandler*)userPointer)->handleAudio((float*)outputBuffer, (float*)inputBuffer, nBufferFrames, streamTime, status);
        };

    if (rtAudio->openStream(&parameters, NULL, RTAUDIO_FLOAT32, sampleRate, &bufferFrames, audioCallback, this))
    {
        std::cout << '\n' << rtAudio->getErrorText() << '\n' << std::endl;
        exit(1);
    }

    if (rtAudio->startStream())
    {
        std::cout << rtAudio->getErrorText() << std::endl;
        exit(1);
    }
}

AudioHandler::~AudioHandler()
{
    if (rtAudio->isStreamRunning())
    {
        rtAudio->stopStream();  // or could call dac.abortStream();
    }

    if (rtAudio->isStreamOpen())
    {
        rtAudio->closeStream();
    }

    delete rtAudio;
}
