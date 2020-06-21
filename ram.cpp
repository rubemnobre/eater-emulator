#include<cstdint>
#include<cstdio>
#include<cstdlib>

namespace RAM{
    const int RAM_SIZE = 16;

    uint8_t* loadFromFile(FILE *ramFile){
        uint8_t *ramPointer = (uint8_t*)malloc(sizeof(uint8_t) * RAM_SIZE);
        fread(ramPointer, sizeof(uint8_t), RAM_SIZE, ramFile);
        return ramPointer;
    }

    uint8_t* loadNull(){
        uint8_t *ramPointer = (uint8_t*)malloc(sizeof(uint8_t) * RAM_SIZE);
        int i;
        for(i = 0; i < RAM_SIZE; i++){
            ramPointer[i] = 0;
        }
        return ramPointer;
    }

    void dump(uint8_t *ramPointer){
        FILE *dump = fopen("dump.ram", "wb");
        int i;
        for(i = 0; i < RAM_SIZE; i++)
            fputc(ramPointer[i], dump);
        fclose(dump);
    }
}