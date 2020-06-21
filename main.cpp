#include "ram.cpp"
#include "logic.cpp"

int main(int argc, char** argv){
    uint8_t* ramPointer = nullptr; //ram array 

    logic CPU(nullptr);

    if(argc < 2){
        printf("Usage: blablabla\n");
        return 1;
    }
    
    int i;
    for(i = 1; i < argc; i++){
        if(argv[i][0] == '-'){
            switch(argv[i][1]){
                case 'f':{ // -f: ram loaded from a file
                    i++;
                    if(i == argc){
                        printf("Missing arguments for -f");
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
                case 'r':{
                    i++;
                    if(i == argc){
                        printf("Missing arguments for -r");
                        return 1;
                    }
                    FILE* romFile = fopen(argv[i], "rb");
                    fread(CPU.instructions, sizeof(uint16_t), 16*STEPS_PER_INSTRUCTION, romFile);
                    fclose(romFile);
                    break;
                }
            }
        }
        if(ramPointer == nullptr){
            printf("Missing RAM init\n");
            return 1;
        }
        
        CPU.ram = ramPointer;

        for(i = 0; i < 15; i++){ //runs the first three instructions
            CPU.cycle();
            printf("%X\n", CPU.outReg);
        }
    }
    return 0;
}