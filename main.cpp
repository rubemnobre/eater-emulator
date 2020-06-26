
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
    logicWindow guiWindow;
    return gui->run(guiWindow);
}
