
#include "ram.h"
#include "logic.h"
#include "logicWindow.h"
#include <gtkmm-3.0/gtkmm/application.h>
#include <array>
#include <getopt.h>
#include <iostream>


int main(int argc, char** argv){
    std::array<std::uint8_t, RAM::RAM_SIZE> ram;
    std::array<std::array<std::uint16_t, STEPS_PER_INSTRUCTION>, 16> instructions = logic<STEPS_PER_INSTRUCTION>::defaultInstructions;
    //logic<STEPS_PER_INSTRUCTION> CPU(RAM::loadNull(), instructions, true);
    std::cout << "Ben Eater's 8-bit Computer Emulator\nMade by Rubem Nobre @ https://github.com/rubemnobre/eater-emulator" << std::endl;
    //FILE* ramFile = fopen("example-programs/fibonacci.ram", "rb");
    //ram = RAM::loadFromFile(ramFile);
    auto gui = Gtk::Application::create(argc, argv, "com.rubemjrx.eater.emulator");
    logicWindow guiWindow;
    //guiWindow.CPU.ram = ram;
    return gui->run(guiWindow);
}
