emulator.bin: main.cpp logic.h ram.h
	$(CXX) $< -o $@
