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
            clockSpeed(5, 510, 10)
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
            
            output.set_text("0");
            outputLabel.set_text("Output:");
            clockLabel.set_text("Clock (Hz): ");
            runClockLabel.set_text("Run Clock: ");
            signedOutLabel.set_text("Signed Out: ");
            
            generalGrid.set_column_homogeneous(true);
            generalGrid.attach(outFrame, 0, 0, 1, 1);
            generalGrid.attach(timeFrame, 0, 1, 1, 1);
            
            outFrame.add(outGrid);
            outFrame.set_border_width(5);
            outGrid.set_column_homogeneous(true);
            outGrid.attach(signedOutLabel, 2, 2, 1, 1);
            outGrid.attach(outputLabel   , 0, 2, 1, 1);
            outGrid.attach(signedOut     , 3, 2, 1, 1);
            outGrid.attach(output        , 1, 2, 2, 1);

            timeFrame.add(timeGrid);
            timeFrame.set_border_width(5);
            timeGrid.set_column_homogeneous(true);
            timeGrid.attach(nextInstructionButton, 0, 1, 1, 1);
            timeGrid.attach(clockLabel           , 0, 2, 1, 1);
            timeGrid.attach(runClockLabel        , 2, 1, 1, 1);
            timeGrid.attach(runFree              , 3, 1, 1, 1);
            timeGrid.attach(nextButton           , 1, 1, 1, 1);
            timeGrid.attach(clockSpeed           , 1, 2, 3, 1);
            
            add(generalGrid);
            show_all_children();
     }
        
        void cpuCycle(){
            if(!CPU.halt){
                CPU.cycle();
                if(!signedOut.get_active())
                    output.set_text(std::to_string(CPU.outReg));
                else
                    output.set_text(std::to_string(static_cast<int8_t>(CPU.outReg)));
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
        Gtk::Switch signedOut;
        Gtk::HScale clockSpeed;
        Gtk::Button nextButton;
        Gtk::Button nextInstructionButton;
        Gtk::Frame timeFrame;
        Gtk::Frame outFrame;
        Gtk::Grid generalGrid;
        Gtk::Grid timeGrid;
        Gtk::Grid outGrid;
        Gtk::Label signedOutLabel;
        Gtk::Label outputLabel;
        Gtk::Label output;
        Gtk::Label clockLabel;
        Gtk::Label runClockLabel;
};


#endif