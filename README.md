This project is a software synthesizer that runs as a desktop app. The audio output and midi input are handled using the RtAudio and RtMidi libraries, and the graphical user interface is built from scratch using OpenGL with GLFW for windowing.

The project is written in C++ and there is some GLSL for the GUI. The GLSL files are written with the vertex and fragment shaders combined into a single file, which are combined with an additional shared section before begin separated and sent to the graphics hardware.
