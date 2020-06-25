#ifndef LOGIC_WINDOW_H
#define LOGIC_WINDOW_H

#include <gtkmm-3.0/gtkmm/window.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include "logic.h"
#include "ram.h"

constexpr int STEPS_PER_INSTRUCTION = 5;

class logicWindow : public Gtk::Window{
    public:
        logic<STEPS_PER_INSTRUCTION> CPU;
        void cycle(){
            CPU.cycle();
        }
        logicWindow() {
            set_default_size(200, 200);
            nextButton = Gtk::Button("cycle");
            nextButton.signal_clicked().connect(sigc::mem_fun(*this, &logicWindow::cycle));
            add(nextButton);
            nextButton.show();
        }
    protected:
        Gtk::Button nextButton;

};

#endif