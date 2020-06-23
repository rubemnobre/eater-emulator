#include <array>

#include "ram.h"
#include "logic.h"

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

    int i;
    for(i = 1; i < argc; i++){
        if(argv[i][0] == '-'){
            switch(argv[i][1]){
                case 'h':
                    printf("%s", helptext);
                    return 0;
                case 'f':{ // -f: ram loaded from a file
                    i++;
                    if(i == argc){
                        printf("Missing arguments for -f\n%s", helptext);
                        return 1;
                    }
                    FILE* ramFile = fopen(argv[i], "rb");
                    ram = RAM::loadFromFile(ramFile);
                    fclose(ramFile);
                    break;
                }
                case 'r':{ // -r load instructions rom from file
                    i++;
                    if(i == argc){
                        printf("Missing arguments for -r\n%s", helptext);
                        return 1;
                    }
                    FILE* romFile = fopen(argv[i], "rb");

                    std::array<std::array<std::uint16_t, STEPS_PER_INSTRUCTION>, 16> instructions;
                    for (auto & instruction: instructions)
                    {
                        for(auto & step: instruction)
                        {
                            fread(&step, sizeof(uint16_t), 1, romFile);
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
            }
        }
    }

    CPU.ram = ram;

    while(!CPU.halt) {
        CPU.cycle();
        if(CPU.outputNow){   //Print the output register if its value changes
            if(SIGNED_OUT)
                printf("%d\n", static_cast<int8_t>(CPU.outReg));
            else
                printf("%d\n", static_cast<uint8_t>(CPU.outReg));
        }
    }
    return 0;
}
