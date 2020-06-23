eater-emulator
============
 Emulator of [Ben Eater's 8-bit computer](https://eater.net/8bit/) made on C++.

 Also thank you to [Joseph Shaker](https://github.com/jshaker000) for the help with making this proper C++.

###  How to build
To build this program, you can run:
- `$ g++ main.cpp -o emulator.bin` on your console .
- Or simply `$ make`.
### How to run your programs
The programs for the emulated computer are run by loading the program into RAM and running the clock. To do this in the emulator, you can create a 16-Byte binary file and load it into the emulator RAM using the `-f` option.  
For example, I have created a few example programs that can be found in the `example-programs/` folder using [Ghex](https://wiki.gnome.org/Apps/Ghex). This is what `fibonacci` looks like:
- In assembly:  
    `LDA[0xD] OUT STA[0xF] ADD[0xE] STA[0xD] LDA[0xF] STA[0xE] JC[0xA] JMP[0x0] HLT`
- In hex:  
    `1D E0 4F 2E 4D 1F 4E 7A 60 FF 00 00 00 01 00 01`
- And when you run it:
    ```
        $ ./emulator.bin -f example-programs/fibonacci.ram
        Ben Eater's 8-bit Computer Emulator
        Made by Rubem Nobre @ https://github.com/rubemnobre/eater-emulator

        1
        1
        2
        3
        5
        8
        13
        21
        34
        55
        89
        144
        233
    ```
### How to run a modified instruction set
You can pass a modified instruction set by using the `-r` option. The instruction set should look like `uint16_t instructions[16][5]` and be in a binary file. I have included the default instruction set in the file `instructions.rom` so you can modify it more easily. For more info on how it works, you can look at the `logic.cpp` file, which is hopefully well documented enough.

### Additional options
Aside from the already mentioned options, I have included these:
 - `-s` : signed output mode. With this, you will see the output as two's compliment signed bytes. Example: `255` will be printed `-1`  
 - `-d` : CPU debug mode. With this, the program will print the current values for every clock cycle of: 
     1. The current microinstructions
     2. The Bus
     3. The Program Counter
     4. The Instruction Register
     5. The current step within the instruction
     6. The Memory Address Register
### Considerations
- In the current version, programs that do not Halt will run MANY times per second and will not time out. To kill them all you can do is press `ctrl+c`.
- In the future, I intend to make a GUI version of this and add clock simulation to make it's timing more realistic.