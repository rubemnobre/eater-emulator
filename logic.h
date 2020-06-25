#ifndef LOGIC_H
#define LOGIC_H
#include <array>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include "ram.h"

template <std::size_t STEPS_PER_INSTRUCTION>
class logic{
    public:
    //The description can be found at the microinstructions method
        static constexpr std::uint16_t FI  = 1 << 0;
        static constexpr std::uint16_t  J  = 1 << 1;
        static constexpr std::uint16_t CO  = 1 << 2;
        static constexpr std::uint16_t CE  = 1 << 3;
        static constexpr std::uint16_t OI  = 1 << 4;
        static constexpr std::uint16_t BI  = 1 << 5;
        static constexpr std::uint16_t SU  = 1 << 6;
        static constexpr std::uint16_t EO  = 1 << 7;
        static constexpr std::uint16_t AO  = 1 << 8;
        static constexpr std::uint16_t AI  = 1 << 9;
        static constexpr std::uint16_t II  = 1 << 10;
        static constexpr std::uint16_t IO  = 1 << 11;
        static constexpr std::uint16_t RO  = 1 << 12;
        static constexpr std::uint16_t RI  = 1 << 13;
        static constexpr std::uint16_t MI  = 1 << 14;
        static constexpr std::uint16_t HLT = 1 << 15;

        static constexpr std::uint8_t JC = 7; //Jump if Carry instruction
        static constexpr std::uint8_t JZ = 8; //Jump if Zero instruction

        static constexpr std::array<std::array<std::uint16_t, STEPS_PER_INSTRUCTION>, 16> defaultInstructions { // Instruction ROM
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE                                      }, // 0x0 NOP: No op
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE, MI | IO, RO | AI                    }, // 0x1 LDA: load from memory to A
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE, MI | IO, RO | BI, EO | AI | FI      }, // 0x2 ADD: load from memory, add to A and put result in A
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE, IO | MI, RO | BI, EO | AI | SU | FI }, // 0x3 SUB: invert the B register's signal in the alu
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE, IO | MI, AO | RI                    }, // 0x4 STA: store A value to ram
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE, IO | AI                             }, // 0x5 LDI: load instruction into A
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE, IO | J                              }, // 0x6 JMP: jump to instruction
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE                                      }, // 0x7 JC: jump if carry
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE                                      }, // 0x8 JZ: jump if zero
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE                                      }, // 0x9 NOP
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE                                      }, // 0xA NOP
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE                                      }, // 0xB NOP
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE                                      }, // 0xC NOP
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE                                      }, // 0xD NOP
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE, AO | OI                             }, // 0xE OUT: Put A in the output Register
            std::array<std::uint16_t, STEPS_PER_INSTRUCTION>{ MI | CO, RO | II | CE, HLT                                 }, // 0xF HLT: set halt flag
        };

        std::array<std::array<std::uint16_t, STEPS_PER_INSTRUCTION>, 16> instructions;

        bool          DEBUG = false;
        bool          halt  = false;
        bool          outputNow = false;
        std::uint8_t  bus; //the main bus (8-bit)
        std::uint8_t  regA; //A register (8-bit)
        std::uint8_t  regB; //B register (8-bit)
        std::uint8_t  flags; //Flags Register
        std::uint8_t  outReg; //Output register (8-bit)
        std::array<std::uint8_t, 16> ram;

        logic(const std::array<std::uint8_t, 16> &nram, std::array<std::array<std::uint16_t, STEPS_PER_INSTRUCTION>, 16> &ninst, const bool nDEBUG){
            ram = nram;
            DEBUG = nDEBUG;
            instructions = ninst;
        }

        logic(){
            ram = RAM::loadNull();
            DEBUG = true;
            instructions = defaultInstructions;
        }

        void debugOut(){
            //printf("\033[2,31mBus: 0x%X, PC: 0x%X, IR: 0x%X, IC: 0x%X, MAR: 0x%X\nREG A: 0x%X REG B: 0x%X OUT: 0x%X\033[0m \n", bus, PC, IR, IC, MAR, regA, regB, outReg);
            std::cout << "\033[0;31m--------------" << std::endl
                      << std::hex << std::showbase << std::right
                      << " Bus: " << static_cast<unsigned int>(bus)
                      << " PC: " << static_cast<unsigned int>(PC)  
                      << " IR: " << static_cast<unsigned int>(IR)
                      << " IC: " << static_cast<unsigned int>(IC)
                      << " MAR: " << std::hex << static_cast<unsigned int>(MAR)
                      << std::endl 
                      << "\033[0;33m"
                      << " A:   " << static_cast<unsigned int>(regA)
                      << " B:  " << static_cast<unsigned int>(regB)
                      << " OUT:" << static_cast<unsigned int>(outReg)
                      << std::endl << "\033[0;36m RAM: ";
            for(auto address : ram){
                std::cout << std::hex << static_cast<unsigned int>(address) << " ";
            }
            std::cout << "\033[0m" << std::endl;
        }

        void cycle() { //Runs a cycle of instructions
            if(DEBUG)
                debugOut();
            microInstructions(instructions[IR >> 4][IC]); //Runs the microinstructions for the current step
            
            IC++; //increment the Instruction step Counter
            if(IC == STEPS_PER_INSTRUCTION){ //Limit steps per Instructions
                IC = 0;
            }
        }
        int currentInstruction() {
            return static_cast<int>(PC);
        }
    private:
        std::uint8_t IR  = 0; //Instruction Register (8-bit)
        std::uint8_t MAR = 0; //Memory Address Register (4-bit)
        std::uint8_t PC  = 0; //Program Counter (4-bit)
        std::uint8_t IC  = 0; //Instruction Counter (0-6)

        void microInstructions(std::uint16_t options){ //Output commands have to come first
            if(DEBUG && options){ //The debug option shows the microinstructions that are selected in the cycle
                printf("\033[1;32m Instructions: ");
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
                    bus = static_cast<std::uint8_t>(regA - regB);
                    if((regA - regB) > 255) //if the output is > 255, write Carry Flag
                        co = 1;
                    if(DEBUG)
                        printf("SU ");
                }else{
                    bus = regA + regB;
                    if((regA + regB) > 255) //if the output is > 255, write Carry Flag
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
            if(options & IO){ //IO (IR Out): puts the 4lsb from IR on the bus
                bus = IR & 0x0f;
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
                outputNow = true;
                if(DEBUG)
                    printf("OI ");
            } else {
                outputNow = false;
            }
            if(options & BI) { //BI (B Reg In): puts the bus value in the B Register
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
                printf("\033[0m\n");
        }
};
#endif
