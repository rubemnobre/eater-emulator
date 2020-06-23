#include <array>

#include "ram.h"
#include "logic.h"

#include <getopt.h>
#include <cstdint>
#include <cstdio>

static constexpr char helptext[] = "usage: \n-f <file> : loads the binary file into RAM\n-r <file> : loads the binary file for the instructions rom\n-d : run with debug option on\n-s : signed output\n";
static constexpr int STEPS_PER_INSTRUCTION = 5;

int main(int argc, char** argv){
    bool SIGNED_OUT = false;
    std::array<std::uint8_t, RAM::RAM_SIZE> ram = RAM::loadNull();
    logic <STEPS_PER_INSTRUCTION> CPU(ram);
    printf("Ben Eater's 8-bit Computer Emulator\nMade by Rubem Nobre @ https://github.com/rubemnobre/eater-emulator\n\n");

    if(argc < 2){
        printf("%s", helptext);
        return 1;
    }

    char c;
    while((c = getopt(argc, argv, "hf:r:sd")) != -1){
        switch (c)
        {
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
                std::array<std::array<std::uint16_t, STEPS_PER_INSTRUCTION>, 16> instructions;
                for (auto & instruction: instructions)
                {
                    for(auto & step: instruction)
                    {
                        fread(&step, sizeof(std::uint16_t), 1, romFile);
                    }
                }
                fclose(romFile);
                CPU.instructions = instructions;
                break;
            }
            case 's': //Set signed output to true
                SIGNED_OUT = true;
                break;
            case 'd':
                CPU.DEBUG = true;
                break;
            default:
                printf("%d", helptext);
        }
    }

    CPU.ram = ram;
    
    while(!CPU.halt) {
        CPU.cycle();
        if(CPU.outputNow){   //Print the output register if its value changes
            if(SIGNED_OUT)
                printf("%d\n", static_cast<std::int8_t>(CPU.outReg));
            else
                printf("%d\n", static_cast<std::uint8_t>(CPU.outReg));
        }
    }
    return 0;
}
