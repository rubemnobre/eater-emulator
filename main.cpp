#include <array>

#include "ram.h"
#include "logic.h"

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

    if(argc < 2){
        printf("%s", helptext);
        return 1;
    }

    char c;
    while((c = getopt(argc, argv, "hf:r:sdt:b:")) != -1){
        switch (c)
        {
            case 'b':{
                std::istringstream optargStr(optarg);
                std::string arg;
                while(std::getline(optargStr, arg, ',')){
                    try{
                        breakPoints.push_back(std::stoi(arg));
                    }catch(std::invalid_argument const &e){
                        printf("Invalid breakpoint\n");
                        return 1;
                    }catch(std::out_of_range const &e){
                        printf("Breakpoint too large\n");
                        return 1;
                    }
                }
                break;
            }
            case 't':
                try{
                    timeout = std::stoi(optarg);   
                }catch(std::invalid_argument const &e){
                    printf("Invalid timeout argument\n");
                    return 1;
                }catch(std::out_of_range const &e){
                    printf("Timeout too large\n");
                    return 1;
                }
                break;
            case 'h':
                printf("%s", helptext);
                return 0;
            case 'f':{ // -f: ram loaded from a file
                FILE* ramFile = fopen(optarg, "rb");
                if(ramFile){
                    ram = RAM::loadFromFile(ramFile);
                    fclose(ramFile);
                }else{
                    std::cout << "Invalid RAM file" << std::endl;
                    return 1;
                }
                break;
            }
            case 'r':{ // -r load instructions rom from file
                FILE* romFile = fopen(optarg, "rb");
                if(romFile){
                    for (auto & instruction: instructions)
                    {
                        for(auto & step: instruction)
                        {
                            fread(&step, sizeof(std::uint16_t), 1, romFile);
                        }
                    }
                    fclose(romFile);
                }else{
                    std::cout << "Invalid ROM file" << std::endl;
                    return 1;
                }
                break;
            }
            case 's': //Set signed output to true
                SIGNED_OUT = true;
                break;
            case 'd':
                DEBUG = true;
                break;
            default:
                printf("%d", helptext);
        }
    }

    logic <STEPS_PER_INSTRUCTION> CPU(ram, instructions, DEBUG);

    int cycles = 0;

    while(!CPU.halt) {
        cycles++;
        CPU.cycle();
        if(CPU.outputNow){   //Print the output register if its value changes
            std::cout << "\033[1;34m OUTPUT: ";
            if(SIGNED_OUT)
                std::cout << static_cast<int>(static_cast<int8_t>(CPU.outReg));
            else
                std::cout << static_cast<unsigned int>(CPU.outReg);
            std::cout << "\033[0m" << std::endl;
        }
        if(CPU.DEBUG){
            std::cout << "Press ENTER to continue... ";
            std::getchar();
        }
        for(auto point : breakPoints){
            if(point == CPU.currentInstruction()){
                CPU.DEBUG = true;
            }else{
                CPU.DEBUG = DEBUG;
            }
        }
        if(cycles/STEPS_PER_INSTRUCTION >= timeout && timeout != -1) break;
    }
    return 0;
}
