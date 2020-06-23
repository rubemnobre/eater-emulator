#include <array>

#include "ram.h"
#include "logic.h"

#include <getopt.h>
#include <cstdint>
#include <cstdio>
#include <string>
#include <iostream>

static constexpr char helptext[] = "usage: \n-f <file> : loads the binary file into RAM\n-r <file> : loads the binary file for the instructions rom\n-d : run with debug option on\n-s : signed output\n-t <n>: timeout after <n> instructions";
static constexpr int STEPS_PER_INSTRUCTION = 5;


int main(int argc, char** argv){
    bool SIGNED_OUT = false;
    bool DEBUG = false;
    bool newInstructions = false;
    int timeout = -1;
    std::array<std::uint8_t, RAM::RAM_SIZE> ram = RAM::loadNull();
    std::array<std::array<std::uint16_t, STEPS_PER_INSTRUCTION>, 16> instructions = logic<STEPS_PER_INSTRUCTION>::defaultInstructions;
    
    printf("Ben Eater's 8-bit Computer Emulator\nMade by Rubem Nobre @ https://github.com/rubemnobre/eater-emulator\n\n");

    if(argc < 2){
        printf("%s", helptext);
        return 1;
    }

    char c;
    while((c = getopt(argc, argv, "hf:r:sdt:")) != -1){
        switch (c)
        {
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
                ram = RAM::loadFromFile(ramFile);
                fclose(ramFile);
                break;
            }
            case 'r':{ // -r load instructions rom from file
                FILE* romFile = fopen(optarg, "rb");
                for (auto & instruction: instructions)
                {
                    for(auto & step: instruction)
                    {
                        fread(&step, sizeof(std::uint16_t), 1, romFile);
                    }
                }
                fclose(romFile);
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
            if(SIGNED_OUT)
                printf("%d\n", static_cast<std::int8_t>(CPU.outReg));
            else
                printf("%d\n", static_cast<std::uint8_t>(CPU.outReg));
        }
        if(cycles/STEPS_PER_INSTRUCTION >= timeout && timeout != -1) break;
    }
    return 0;
}
