#include "midiHandler.hpp"

void midiCallback(double deltaTime, std::vector<unsigned char>* messageBytes, void* userPointer)
{
    MidiHandler* midiHandler = (MidiHandler*)userPointer;
    int firstByte = (int)messageBytes->at(0);
    int type = firstByte & 0x000000ff;
    int channel = firstByte & 0x0000ff00;

    MidiMessage message;
    message.channel = channel;

    if (type == 144)
    {
        message.type = MidiMessageType::NoteOn;
        message.note = (int)messageBytes->at(1);
        message.velocity = (int)messageBytes->at(2);
    }
    else if (type == 128)
    {
        message.type = MidiMessageType::NoteOff;
        message.note = (int)messageBytes->at(1);
        message.velocity = 0;
    }
    else if (type == 176)
    {
        message.type = MidiMessageType::CC;
        message.cc = (int)messageBytes->at(1);
        message.value = (int)messageBytes->at(2);
    }

    midiHandler->sharedData->messages.push(message);
}

MidiHandler::MidiHandler()
{
    RtMidiIn* midiIn = NULL;

    try
    {
        midiIn = new RtMidiIn();
    }
    catch (RtMidiError& error)
    {
        error.printMessage();
        exit(1);
    }

    std::string midiInputName = "Oxygen Pro Mini 1";
    //std::string midiInputName = "MIDIIN2 (Oxygen Pro Mini) 1";
    //std::string midiInputName = "MIDIIN4 (Oxygen Pro Mini) 3";
    int midiPort = -1;

    unsigned int inputPorts = midiIn->getPortCount();

    for (unsigned int i = 0; i < inputPorts; i++)
    {
        std::string portName;

        try
        {
            portName = midiIn->getPortName(i);
        }
        catch (RtMidiError& error)
        {
            error.printMessage();
            delete midiIn;
            exit(1);
        }
        //std::cout << "  Input Port #" << i << ": " << portName << '\n';
    }

    for (unsigned int i = 0; i < inputPorts; i++)
    {
        std::string portName;

        try
        {
            portName = midiIn->getPortName(i);

            if (portName == midiInputName)
            {
                midiPort = i;
                break;
            }
        }
        catch (RtMidiError& error)
        {
            error.printMessage();
            delete midiIn;
            exit(1);
        }
    }

    midiIn->openPort(midiPort);
    midiIn->setCallback(&midiCallback, this);

    std::vector<unsigned char> message;
    while (message.size() != 0) // Empty the message queue
    {
        midiIn->getMessage(&message);
    }

    //midiIn->ignoreTypes(false, false, false); // sysex, timing, active sensing are ignored by default
}

MidiHandler::~MidiHandler()
{
    delete midiIn;
}
