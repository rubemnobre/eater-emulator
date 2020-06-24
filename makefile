FLAGS = `pkg-config gtkmm-3.0 --cflags --libs`
emulator.bin: main.cpp logic.h ram.h
	$(CXX) $< -o $@ $(FLAGS)
