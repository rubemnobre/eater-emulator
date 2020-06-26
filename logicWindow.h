#ifndef LOGIC_WINDOW_H
#define LOGIC_WINDOW_H

#include <gtkmm-3.0/gtkmm.h>
#include "logic.h"
#include "ram.h"
#include <iomanip>

constexpr int STEPS_PER_INSTRUCTION = 5;


class logicWindow : public Gtk::Window{
    public:
        logicWindow() :
            nextButton("Clock Cycle"),
            nextInstructionButton("Next Instruction"),
            ramLoad("Load"),
            clockSpeed(5, 510, 10),
            ramAddress(Gtk::Adjustment::create(0, 0, 16, 1, 1, 1))
        {
            set_border_width(10);
            set_title("eater-emulator");

            sigc::slot<bool> slot = sigc::bind(sigc::mem_fun(*this, &logicWindow::update_callback), 1);
            updateConn = Glib::signal_timeout().connect(slot, 100);

            runFree.signal_state_flags_changed().connect(sigc::mem_fun(*this, &logicWindow::set_runFree));

            nextInstructionButton.signal_clicked().connect(sigc::mem_fun(*this, &logicWindow::cpu_inst_callback));
            nextInstructionButton.set_border_width(10);

            ramValue.signal_activate().connect(sigc::mem_fun(*this, &logicWindow::change_ram));
            ramAddress.signal_activate().connect(sigc::mem_fun(ramValue, &Gtk::Widget::grab_focus));
            ramLoad.signal_clicked().connect(sigc::mem_fun(*this, &logicWindow::load_ram));

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
            addressLabel.set_text("RAM Address: ");
            valueLabel.set_text("Current Value (hex): ");
            fileLabel.set_text("Load RAM File: ");
            
            //generalGrid.set_column_homogeneous(true);
            generalGrid.attach(outFrame , 0, 0, 1, 1);
            generalGrid.attach(ramFrame , 1, 0, 1, 1);
            generalGrid.attach(timeFrame, 0, 1, 1, 1);
            
            ramLoad.set_border_width(5);
            ramFileChooser.set_border_width(5);
            ramFileChooser.signal_file_set().connect(sigc::mem_fun(*this, &logicWindow::open_ram_file));

            ramAddress.set_margin_bottom(5);
            ramValue.set_margin_bottom(5);
            ramAddress.set_margin_left(5);
            ramValue.set_margin_left(5);
            ramValue.set_margin_right(5);
            

            ramFrame.add(ramGrid);
            ramFrame.set_border_width(5);
            ramGrid.set_column_homogeneous(true);
            ramGrid.attach(fileLabel     , 0, 0, 1, 1);
            ramGrid.attach(ramFileChooser, 1, 0, 2, 1);
            ramGrid.attach(addressLabel  , 0, 1, 1, 1);
            ramGrid.attach(ramAddress    , 1, 1, 1, 1);
            ramGrid.attach(valueLabel    , 2, 1, 1, 1);
            ramGrid.attach(ramValue      , 3, 1, 1, 1);
            ramGrid.attach(ramLoad       , 3, 0, 1, 1);
            
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
    protected:
        logic<STEPS_PER_INSTRUCTION> CPU;

        sigc::connection timeoutConn;
        sigc::connection updateConn;
        
        Gtk::Grid generalGrid;

        Gtk::Frame ramFrame;
        Gtk::Grid ramGrid;
        Gtk::FileChooserButton ramFileChooser;
        Gtk::SpinButton ramAddress;
        Gtk::Entry ramValue;
        Gtk::Label valueLabel;
        Gtk::Label addressLabel;
        Gtk::Label fileLabel;
        Gtk::Button ramLoad;

        Gtk::Frame outFrame;
        Gtk::Grid outGrid;
        Gtk::Label signedOutLabel;
        Gtk::Label outputLabel;
        Gtk::Label output;
        Gtk::Switch signedOut;

        Gtk::Frame timeFrame;
        Gtk::Grid timeGrid;
        Gtk::Button nextButton;
        Gtk::Button nextInstructionButton;
        Gtk::HScale clockSpeed;
        Gtk::Label clockLabel;
        Gtk::Label runClockLabel;
        Gtk::Switch runFree;

        void change_ram(){
            try{
                CPU.ram[ramAddress.get_value_as_int()] = static_cast<uint8_t>(std::stoi(ramValue.get_text(), 0, 16));
            }catch(std::exception e){
                std::cout << e.what() << std::endl;
            }
            ramAddress.grab_focus_without_selecting();
            //return false;
        }

        void load_ram(){
            if(ramFileChooser.get_file())
                open_ram_file();
            else
                CPU.ram = RAM::loadNull();
        }

        void open_ram_file(){
            uint8_t ram[16];
            ramFileChooser.get_file()->read()->read(ram, 16);
            int i = 0;
            CPU = logic<STEPS_PER_INSTRUCTION>();
            for(auto &value : CPU.ram)
                value = ram[i++];
        }

        void cpuCycle(){
            if(!CPU.halt){
                CPU.cycle();
                out_reg_update();
            }
        }

        void out_reg_update(){
            if(!signedOut.get_active())
                output.set_text(std::to_string(CPU.outReg));
            else
                output.set_text(std::to_string(static_cast<int8_t>(CPU.outReg)));
        }

        void update_ram_value(){
            std::stringstream strm;
            strm << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(CPU.ram[ramAddress.get_value()]);
            ramValue.set_text(std::string(strm.str()));
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

        bool update_callback(int arg){
            if(!ramValue.is_focus())
                update_ram_value();
            out_reg_update();
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
};


#endif