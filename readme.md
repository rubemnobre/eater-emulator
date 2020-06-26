eater-emulator
============
 Emulator of [Ben Eater's 8-bit computer](https://eater.net/8bit/) made on C++.
 This computer has an architecture very similar to SAP-1, but with a 16-bit control-word and a Flags register (which means it can do logical operations).

 Also thank you to [Joseph Shaker](https://github.com/jshaker000) for the help with making this proper C++.

###  How to build
To build this program, you can run:
- ``$ g++ main.cpp -o emulator.bin `pkg-config gtkmm-3.0 --cflags --libs`` on your console. You need to have gtkmm-3.0 and all it's dependencies installed.
- Or simply `$ make` on Linux.
### How to run your programs
The programs for the emulated computer are run by loading the program into RAM and running the clock. To do this in the emulator, you can create a 16-Byte binary file and load it into the emulator RAM using the Load RAM File option in the gui.  
For example, I have created a few example programs that can be found in the `example-programs/` folder using [Ghex](https://wiki.gnome.org/Apps/Ghex). This is what `fibonacci` looks like:
- In assembly:  
    `LDA[0xD] OUT STA[0xF] ADD[0xE] STA[0xD] LDA[0xF] STA[0xE] JC[0xA] JMP[0x0] HLT`
- In hex:  
    `1D E0 4F 2E 4D 1F 4E 7A 60 FF 00 00 00 01 00 01`
- And when you run it:
    ![example](https://i.imgur.com/mTTH2c0.gif)
### How to run a modified instruction set
You can pass a modified instruction set by modifying the `instructions.rom` file. The instruction set should look like `uint16_t instructions[16][5]` and be in a binary file. For more info on how it works, you can look at the `logic.h` file for the control words and microcode, which is hopefully well documented enough.
