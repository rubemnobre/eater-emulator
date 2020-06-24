
#include "ram.h"
#include "logic.h"
#include <gtkmm.h>
#include <array>
#include <getopt.h>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

static constexpr char helptext[] = "usage: \n-f <file> : loads the binary file into RAM\n-r <file> : loads the binary file for the instructions rom\n-d : run with debug option on\n-s : signed output\n-t n : timeout after n instructions\n-b a,b,... : add breakpoints when the program counter is at one of the specified values\n";
static constexpr int STEPS_PER_INSTRUCTION = 5;

int main(int argc, char** argv){
    bool SIGNED_OUT = false;
    bool DEBUG = false;
    bool newInstructions = false;
    int timeout = -1;
    std::array<std::uint8_t, RAM::RAM_SIZE> ram = RAM::loadNull();
    std::array<std::array<std::uint16_t, STEPS_PER_INSTRUCTION>, 16> instructions = logic<STEPS_PER_INSTRUCTION>::defaultInstructions;
    std::vector<int> breakPoints;

    printf("Ben Eater's 8-bit Computer Emulator\nMade by Rubem Nobre @ https://github.com/rubemnobre/eater-emulator\n\n");

    auto gui = Gtk::Application::create(argc, argv, "com.rubemjrx.eater.emulator");
    Gtk::Window guiWindow;
    guiWindow.set_default_size(200, 200);

    return gui->run(guiWindow);

    logic <STEPS_PER_INSTRUCTION> CPU(ram, instructions, DEBUG);
    
    return 0;
}
