#include<cstdint>
#include<cstdio>

bool DEBUG = false;

const int STEPS_PER_INSTRUCTION = 5;
const uint16_t  J = 1 << 0;
const uint16_t CO = 1 << 1;
const uint16_t CE = 1 << 2;
const uint16_t OI = 1 << 3;
const uint16_t BI = 1 << 4;
const uint16_t SU = 1 << 5;
const uint16_t EO = 1 << 6;
const uint16_t AO = 1 << 7;
const uint16_t AI = 1 << 8;
const uint16_t II = 1 << 9;
const uint16_t IO = 1 << 10;
const uint16_t RO = 1 << 11;
const uint16_t RI = 1 << 12;
const uint16_t MI = 1 << 13;
const uint16_t HLT = 1 << 14;

class logic{
    public:
        uint8_t bus; //the main bus (8-bit)
        uint8_t regA; //A register (8-bit)
        uint8_t regB; //B register (8-bit)
        uint8_t outReg; //Output register (8-bit)
        uint8_t *ram; //16 Bytes
        uint16_t instructions[16][STEPS_PER_INSTRUCTION] = //Equivalent to the Instruction ROM
        {
            { MI | CO, RO | II | CE, 0, 0, 0 },
            { MI | CO, RO | II | CE, MI | IO, RO | AI, 0 }, //LDA: load from memory to A
            { MI | CO, RO | II | CE, MI | IO, RO | BI, EO | AI }, //ADD: load from memory, add to A and put result in A
            { MI | CO, RO | II | CE, 0, 0, 0 },
            { MI | CO, RO | II | CE, 0, 0, 0 },
            { MI | CO, RO | II | CE, 0, 0, 0 },
            { MI | CO, RO | II | CE, 0, 0, 0 },
            { MI | CO, RO | II | CE, 0, 0, 0 },
            { MI | CO, RO | II | CE, 0, 0, 0 },
            { MI | CO, RO | II | CE, 0, 0, 0 },
            { MI | CO, RO | II | CE, 0, 0, 0 },
            { MI | CO, RO | II | CE, 0, 0, 0 },
            { MI | CO, RO | II | CE, 0, 0, 0 },
            { MI | CO, RO | II | CE, 0, 0, 0 },
            { MI | CO, RO | II | CE, AO | OI, 0, 0 }, //OUT: Put A in the output Register
            { MI | CO, RO | II | CE, 0, 0, 0 },
        };
        logic(uint8_t *nram){
            ram = nram;
        }

        void cycle(){ //Runs a cycle of instructions
            microInstructions(instructions[IR >> 4][IC]); //Runs the microinstructions for the current step
            if(DEBUG)
                printf("bus: %X, pc: %X, ir: %X, mar: %X\n", bus, PC, IR, MAR);
            IC++; //increment the Instruction step Counter
            if(IC == STEPS_PER_INSTRUCTION){ //Limit steps per Instructions
                IC = 0;
            }
        }
    private:
        uint8_t IR = 0; //Instruction Register (8-bit)
        uint8_t MAR = 0; //Memory Address Register (4-bit)
        uint8_t PC = 0; //Program Counter (4-bit)
        uint8_t IC = 0; //Instruction Counter (0-6)

        void microInstructions(uint16_t options){ //Output commands have to come first
            if(DEBUG && options){
                printf("Instructions: ");
            }
            if((options >> 0x1) & 1){ //CO (Counter Out): puts the content of PC into the Bus
                bus = PC;
                if(DEBUG)
                    printf("CO ");
            }
            if((options >> 0x6) & 1){ //EO (Sum Out): puts the regA + regB value in the bus (simulating the ALU's job)
                bus = regA + regB;
                if(DEBUG)
                    printf("EO ");
            }
            if((options >> 0x7) & 1){ //AO (A Reg Out): puts the A Register value in the bus
                bus = regA;
                if(DEBUG)
                    printf("AO ");
            }
            if((options >> 0xB) & 1){ //RO (Ram Out): puts the value in ram pointed to by the MAR in the bus
                bus = ram[MAR];
                if(DEBUG)
                    printf("RO ");
            }
            if((options >> 0xA) & 1){ //IO (IR Out): puts IR on the bus
                bus = IR;
                if(DEBUG)
                    printf("IO ");
            }
            if((options >> 0x3) & 1){ //OI (Output Reg In): puts the bus value in the Output Register
                outReg = bus;
                if(DEBUG)
                    printf("OI ");
            }
            if((options >> 0x4) & 1){ //BI (B Reg In): puts the bus value in the B Register
                regB = bus;
                if(DEBUG)
                    printf("BI ");
            }
            if((options >> 0x8) & 1){ //AI (A Reg In): puts the bus value in the A Register
                regA = bus;
                if(DEBUG)
                    printf("AI ");
            }
            if((options >> 0x9) & 1){ //II (IR In): puts the bus value into the IR
                IR = bus;
                if(DEBUG)
                    printf("II ");
            }
            if((options >> 0xD) & 1){ //MI (MAR In): reads the address from the bus to the Memory Address Register
                MAR = bus & 0x0F; //This AND operation assures the MAR does not read the higher nibble
                if(DEBUG)
                    printf("MI ");
            }
            if((options >> 0x2) & 1){ //CE (Counter Enable): increments the PC
                PC++;   
                if(DEBUG)
                    printf("CE ");
            }
            if (DEBUG && options)
                printf("\n");
        }
};