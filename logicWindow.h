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
            nextButton("Clock Cycle"),
            nextInstructionButton("Next Instruction"),
            clockSpeed(5, 510, 10),
            output("0")
        {
            set_border_width(10);
            set_title("eater-emulator");

            runFree.signal_state_flags_changed().connect(sigc::mem_fun(*this, &logicWindow::set_runFree));

            nextInstructionButton.signal_clicked().connect(sigc::mem_fun(*this, &logicWindow::cpu_inst_callback));
            nextInstructionButton.set_border_width(10);

            nextButton.signal_clicked().connect(sigc::mem_fun(*this, &logicWindow::cpuCycle));
            nextButton.set_border_width(10);
            
            clockSpeed.get_adjustment()->signal_value_changed().connect(sigc::mem_fun(*this, &logicWindow::set_clock));
            clockSpeed.set_margin_bottom(10);
            clockSpeed.set_margin_right(10);
            
            output.set_text("Output: 0");
            clockLabel.set_text("Clock (Hz): ");
            runClockLabel.set_text("Run Clock: ");

            timeFrame.add(timeGrid);
            add(timeFrame);
            timeGrid.attach(nextInstructionButton, 0, 1, 1, 1);
            timeGrid.attach(output               , 0, 0, 4, 1);
            timeGrid.attach(clockLabel           , 0, 2, 1, 1);
            timeGrid.attach(runClockLabel        , 2, 1, 1, 1);
            timeGrid.attach(runFree              , 3, 1, 1, 1);
            timeGrid.attach(nextButton           , 1, 1, 1, 1);
            timeGrid.attach(clockSpeed           , 1, 2, 3, 1);

            show_all_children();
        }
        
        void cpuCycle(){
            if(!CPU.halt){
                CPU.cycle();
                output.set_text("Output: " + std::to_string(CPU.outReg));
            }
        }
        void cpu_inst_callback(){
            int i;
            for(int i = 0; i < 5; i++)
                cpuCycle();
        }
        bool cpu_cycle_callback(int arg){
            cpuCycle();
            return true;
        }
        void set_clock(){
            if(runFree.get_active()){
                if(timeoutConn.connected()){
                    timeoutConn.disconnect();
                }
                unsigned int interval = 1000/clockSpeed.get_value(); 
                sigc::slot<bool> cycleSlot = sigc::bind(sigc::mem_fun(*this, &logicWindow::cpu_cycle_callback), 1);
                timeoutConn = Glib::signal_timeout().connect(cycleSlot, interval);
            }else{
                timeoutConn.disconnect();
            }
        }
        void set_runFree(Gtk::StateFlags flags){
            set_clock();
        }
        
        
    protected:
        sigc::connection timeoutConn;
        Gtk::Switch runFree;
        Gtk::HScale clockSpeed;
        Gtk::Button nextButton;
        Gtk::Button nextInstructionButton;
        Gtk::Frame timeFrame;
        Gtk::Grid timeGrid;
        Gtk::Label output;
        Gtk::Label clockLabel;
        Gtk::Label runClockLabel;
};


#endif