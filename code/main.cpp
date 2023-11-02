#include "audio/audioHandler.hpp"
#include "audio/midiHandler.hpp"
#include "audio/audioUtility.hpp"
#include "audio/shared.hpp"
#include "audio/audioGenerator.hpp"
#include "audio/sineAudioGenerator.hpp"
#include "audio/fmAudioGenerator.hpp"
#include "graphics/gui.hpp"
#include "controls.hpp"
#include <string>

std::string projectPath = "D:/Programming/Visual Studio 2022/FM Synth/";

int main()
{
    Controls controls;
    SharedData sharedData;
    //SineAudioGenerator sine;
    FMAudioGenerator gen;
    ADSR envelope(controls);

    /*envelope.attack = 0.01f;
    envelope.decay = 1;
    envelope.sustain = 0.5f;
    envelope.release = 1;*/

    /*envelope.attackControl->setValue(0.001);
    envelope.decayControl->setValue(0);
    envelope.sustainControl->setValue(1);
    envelope.releaseControl->setValue(0.5);*/

    envelope.attackControl->setValue(0.001);
    envelope.decayControl->setValue(2);
    envelope.sustainControl->setValue(0.8);
    envelope.releaseControl->setValue(0.1);
    
    //sine.envelope = &envelope;
    //sharedData.generator = &sine;
    gen.envelope = &envelope;
    sharedData.generator = &gen;

    AudioHandler audio;
    audio.sharedData = &sharedData;

    MidiHandler midi;
    midi.sharedData = &sharedData;

    GUI gui;
    gui.start(controls);

    return 0;
}
