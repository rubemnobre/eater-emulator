#include<cstdint>
#include<cstdio>
#include<cstdlib>

namespace RAM{
    const int RAM_SIZE = 32;

    uint8_t* loadRamFile(FILE *ramFile){
        uint8_t *ramPointer = (uint8_t*)malloc(sizeof(uint8_t) * RAM_SIZE);
        fread(ramPointer, sizeof(uint8_t), RAM_SIZE, ramFile);
        return ramPointer;
    }

    uint8_t* nullRam(){
        return (uint8_t*)malloc(sizeof(uint8_t) * RAM_SIZE);
    }
};