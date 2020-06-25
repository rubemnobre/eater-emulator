#ifndef LOGIC_WINDOW_H
#define LOGIC_WINDOW_H

#include <gtkmm-3.0/gtkmm.h>
#include "logic.h"
#include "ram.h"

constexpr int STEPS_PER_INSTRUCTION = 5;


class logicWindow : public Gtk::Window{
    public:
        logic<STEPS_PER_INSTRUCTION> CPU;
        logicWindow() :
            nextButton("cycle"),
            clockSpeed(6, 600, 10),
            output("0")
        {
            set_border_width(10);
            set_title("eater-emulator");
            
            nextButton.signal_clicked().connect(sigc::mem_fun(CPU, &logic<STEPS_PER_INSTRUCTION>::cycle));
            nextButton.set_border_width(10);

            runFree.signal_clicked().connect(sigc::mem_fun(*this, &logicWindow::set_runFree));
            
            clockSpeed.get_adjustment()->signal_value_changed().connect(sigc::mem_fun(*this, &logicWindow::set_runFree));
            
            add(grid);
            grid.attach(runFree   , 1, 0, 1, 1);
            grid.attach(nextButton, 0, 0, 1, 1);
            grid.attach(clockSpeed, 0, 1, 2, 1);

            show_all_children();
        }
        bool cpuCycle(int arg1){
            if(!CPU.halt){
                CPU.cycle();
            }
            return true;
        }
        void set_runFree(){
            if(runFree.get_active()){
                if(timeoutConn.connected()){
                    timeoutConn.disconnect();
                }
                unsigned int interval = 1000/clockSpeed.get_value(); 
                sigc::slot<bool> cycleSlot = sigc::bind(sigc::mem_fun(*this, &logicWindow::cpuCycle), 0);
                timeoutConn = Glib::signal_timeout().connect(cycleSlot, interval);
            }else{
                timeoutConn.disconnect();
            }
        }
    protected:
        sigc::connection timeoutConn;
        Gtk::CheckButton runFree;
        Gtk::HScale clockSpeed;
        Gtk::Button nextButton;
        Gtk::Grid grid;
        Gtk::Label output;
};

#endif