#include<cstdint>
#include<cstdio>

const int STEPS_PER_INSTRUCTION = 5;

//The description can be found at the microinstructions method
const uint16_t FI = 1 << 0;
const uint16_t  J = 1 << 1;
const uint16_t CO = 1 << 2;
const uint16_t CE = 1 << 3;
const uint16_t OI = 1 << 4;
const uint16_t BI = 1 << 5;
const uint16_t SU = 1 << 6;
const uint16_t EO = 1 << 7;
const uint16_t AO = 1 << 8;
const uint16_t AI = 1 << 9;
const uint16_t II = 1 << 10;
const uint16_t IO = 1 << 11;
const uint16_t RO = 1 << 12;
const uint16_t RI = 1 << 13;
const uint16_t MI = 1 << 14;
const uint16_t HLT = 1 << 15;

const uint8_t JC = 7; //Jump if Carry instruction
const uint8_t JZ = 8; //Jump if Zero instruction

class logic{
    public:
        bool DEBUG = false;
        bool halt = false;
        uint8_t bus; //the main bus (8-bit)
        uint8_t regA; //A register (8-bit)
        uint8_t regB; //B register (8-bit)
        uint8_t flags; //Flags Register
        int8_t outReg; //Output register (8-bit)
        uint8_t *ram; //16 Bytes
        uint16_t instructions[16][STEPS_PER_INSTRUCTION] = //Equivalent to the Instruction ROM
        {
            { MI | CO, RO | II | CE, 0,       0,       0                 }, //0 NOP: No op
            { MI | CO, RO | II | CE, MI | IO, RO | AI, 0                 }, //1 LDA: load from memory to A
            { MI | CO, RO | II | CE, MI | IO, RO | BI, EO | AI | FI      }, //2 ADD: load from memory, add to A and put result in A
            { MI | CO, RO | II | CE, IO | MI, RO | BI, EO | AI | SU | FI }, //3 SUB: invert the B register's signal in the alu
            { MI | CO, RO | II | CE, IO | MI, AO | RI, 0                 }, //4 STA: store A value to ram
            { MI | CO, RO | II | CE, IO | AI, 0,       0                 }, //5 LDI: load instruction into A
            { MI | CO, RO | II | CE, IO | J,  0,       0                 }, //6 JMP: jump to instruction
            { MI | CO, RO | II | CE, 0,       0,       0                 }, //7 JC: jump if carry
            { MI | CO, RO | II | CE, 0,       0,       0                 }, //8 JZ: jump if zero
            { MI | CO, RO | II | CE, 0,       0,       0                 }, //9 NOP
            { MI | CO, RO | II | CE, 0,       0,       0                 }, //A NOP
            { MI | CO, RO | II | CE, 0,       0,       0                 }, //B NOP
            { MI | CO, RO | II | CE, 0,       0,       0                 }, //C NOP
            { MI | CO, RO | II | CE, 0,       0,       0                 }, //D NOP
            { MI | CO, RO | II | CE, AO | OI, 0,       0                 }, //E OUT: Put A in the output Register
            { MI | CO, RO | II | CE, HLT,     0,       0                 }, //F HLT: set halt flag
        };
        logic(uint8_t *nram){
            ram = nram;
        }

        void cycle(){ //Runs a cycle of instructions
            microInstructions(instructions[IR >> 4][IC]); //Runs the microinstructions for the current step
            if(DEBUG)
                printf("Bus: 0x%X, PC: 0x%X, IR: 0x%X, IC: 0x%X, MAR: 0x%X, \n", bus, PC, IR, IC, MAR);
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
            if(DEBUG && options){ //The debug option shows the microinstructions that are selected in the cycle
                printf("Instructions: ");
            }
            if(IR >> 4 == JC && flags & 1 && IC == 2){ //Set up the JC instruction if CF is set. If not it is no op
                if(DEBUG)
                    printf("JC True ");
                options = options | IO | J;
            }
            if(IR >> 4 == JZ && flags & 2 && IC == 2){ //Set up the ZC instruction if ZF is set. If not it is no op
                if(DEBUG)
                    printf("JZ True ");
                options = options | IO | J;
            }
            if(options & CO){ //CO (Counter Out): puts the content of PC into the Bus
                bus = PC;
                if(DEBUG)
                    printf("CO ");
            }
            if(options & EO){ //EO (Sum Out): puts the regA + regB value in the bus (simulating the ALU's job)
                int co = 0;
                if(options & SU){ //SU (Subtraction Out): inverts the B value (simulating the ALU's job)
                    bus = (uint8_t)((int)regA - (int)regB);
                    if(((int)regA - (int)regB) > 255) //if the output is > 255, write Carry Flag
                        co = 1;
                    if(DEBUG)
                        printf("SU ");
                }else{
                    bus = regA + regB;
                    if(((int)regA + (int)regB) > 255) //if the output is > 255, write Carry Flag
                        co = 1;
                }
                if(options & FI){
                    if(DEBUG)
                        printf("FI ");
                    flags = co | (bus?0:2);  //join the Carry flag and the Zero flag
                }
                if(DEBUG)
                    printf("EO ");
            }
            if(options & AO){ //AO (A Reg Out): puts the A Register value in the bus
                bus = regA;
                if(DEBUG)
                    printf("AO ");
            }
            if(options & RO){ //RO (Ram Out): puts the value in ram pointed to by the MAR in the bus
                bus = ram[MAR];
                if(DEBUG)
                    printf("RO ");
            }
            if(options & IO){ //IO (IR Out): puts IR on the bus
                bus = IR;
                if(DEBUG)
                    printf("IO ");
            }
            if(options & J){ //J (PC In): puts the bus value in the Program Counter
                PC = bus;
                if(DEBUG)
                    printf("J ");
            }
            if(options & OI){ //OI (Output Reg In): puts the bus value in the Output Register
                outReg = bus;
                if(DEBUG)
                    printf("OI ");
            }
            if(options & BI){ //BI (B Reg In): puts the bus value in the B Register
                regB = (int)bus;
                if(DEBUG)
                    printf("BI ");
            }
            if(options & AI){ //AI (A Reg In): puts the bus value in the A Register
                regA = bus;
                if(DEBUG)
                    printf("AI ");
            }
            if(options & II){ //II (IR In): puts the bus value into the IR
                IR = bus;
                if(DEBUG)
                    printf("II ");
            }
            if(options & RI){ //RI (RAM In): puts the bus value into the RAM
                ram[MAR] = bus;
                if(DEBUG)
                    printf("II ");
            }
            if(options & MI){ //MI (MAR In): reads the address from the bus to the Memory Address Register
                MAR = bus & 0x0F; //This AND operation assures the MAR does not read the higher nibble
                if(DEBUG)
                    printf("MI ");
            }
            if(options & HLT){ //HLT (Halt): Sets the halt flag (so the clock knows to stop)
                halt = true;
                if(DEBUG)
                    printf("HLT ");
            }
            if(options & CE){ //CE (Counter Enable): increments the PC
                PC++;   
                if(DEBUG)
                    printf("CE ");
            }
            if (DEBUG && options)
                printf("\n");
        }
};