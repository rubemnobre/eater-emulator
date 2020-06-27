/*
    Ben Eater's 8-bit Computer Emulator. Made by Rubem Nobre @ https://github.com/rubemnobre/eater-emulator
    Copyright (C) 2020 Rubem Nobre

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
