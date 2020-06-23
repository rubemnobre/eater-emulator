#include "ram.cpp"
#include "logic.cpp"

const char helptext[] = "usage: \n-f <file> : loads the binary file into RAM\n-r <file> : loads the binary file for the instructions rom\n-d : run with debug option on\n-s : signed output";


int main(int argc, char** argv){
    bool SIGNED_OUT = false;
    uint8_t* ramPointer = nullptr; //ram array 

    logic CPU(nullptr);

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
                    ramPointer = RAM::loadFromFile(ramFile);
                    fclose(ramFile);
                    break;
                }
                case 'n': // -n load ram as all zeroes
                    ramPointer = RAM::loadNull();
                    break;
                case 'r':{ // -r load instructions rom from file
                    i++;
                    if(i == argc){
                        printf("Missing arguments for -r\n%s", helptext);
                        return 1;
                    }
                    FILE* romFile = fopen(argv[i], "rb");
                    fread(CPU.instructions, sizeof(uint16_t), 16*STEPS_PER_INSTRUCTION, romFile);
                    fclose(romFile);
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
    if(ramPointer == nullptr){
        printf("Missing RAM init\n%s", helptext);
        return 1;
    }

    CPU.ram = ramPointer;
    while(!CPU.halt){ //runs until the CPU halts
        CPU.cycle();
        if(CPU.outputNow){
            if(SIGNED_OUT)
                printf("%d\n", (int8_t)CPU.outReg); //Print the output register if its value changes
            else
                printf("%d\n", (uint8_t)CPU.outReg); //Print the output register if its value changes
        }
    }
    free(ramPointer);
    return 0;
}