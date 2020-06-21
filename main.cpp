#include "ram.cpp"

int main(int argc, char** argv){
    uint8_t* ramPointer;

    if(argc < 2){
        printf("Usage: blablabla\n");
        return 1;
    }
    
    int i;
    for(i = 1; i < argc; i++){
        if(argv[i][0] == '-'){
            switch(argv[i][1]){
                case 'f':{
                    i++;
                    if(i == argc){
                        printf("Missing arguments for -f");
                        return 1;
                    }
                    FILE* ramFile = fopen(argv[i], "rb");
                    ramPointer = RAM::loadRamFile(ramFile);
                    fclose(ramFile);
                    break;
                }
                case 'n':
                    ramPointer = RAM::nullRam();
                    break;
            }
        }
    }
    
    return 0;
}