
#include "ram.h"
#include "logic.h"
#include "logicWindow.h"
#include <gtkmm-3.0/gtkmm/application.h>
#include <array>
#include <getopt.h>
#include <iostream>


int main(int argc, char** argv){
    std::cout << "Ben Eater's 8-bit Computer Emulator\nMade by Rubem Nobre @ https://github.com/rubemnobre/eater-emulator" << std::endl;
    auto gui = Gtk::Application::create(argc, argv, "com.rubemjrx.eater.emulator");
    auto instructionsFromFile = logic<5>::defaultInstructions;
    FILE* romFile = fopen("instructions.rom", "rb");
    if(romFile){
        for (auto &instruction: instructionsFromFile)
        {
            for(auto &step: instruction)
            {
                fread(&step, sizeof(std::uint16_t), 1, romFile);
            }
        }
        fclose(romFile);
    }else{
        std::cout << "Invalid ROM file" << std::endl;
        return 1;
    }
    logicWindow guiWindow;
    guiWindow.CPU.instructions = instructionsFromFile;
    return gui->run(guiWindow);
}
