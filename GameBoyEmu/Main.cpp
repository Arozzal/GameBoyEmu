#include <iostream>
#include <SDL.h>

#include "IOMap.hpp"
#include "Input.hpp"
#include "Memory.hpp"
#include "Cpu.hpp"
#include "Gpu.hpp"
#include "Timer.hpp"







int main(int argc, char *argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);
	
	IOMap Iomap;
	Memory memory;
	Registers registers;
	Input input;
	Cpu cpu;
	Gpu gpu;
	Timer timer;

	cpu.setRegister(&registers);
	cpu.setMemory(&memory);
	cpu.setInput(&input);
	memory.setGpu(&gpu);
	memory.setReg(&registers);
	memory.setIoMap(&Iomap);
	memory.setInput(&input);
	memory.setCpu(&cpu);
	memory.setTimer(&timer);
	gpu.setIo(&Iomap);
	gpu.setRegisters(&registers);
	gpu.setMemory(&memory);
	timer.setRegisters(&registers);
	timer.setIoMap(&Iomap);

	cpu.init();
	memory.init();

	memory.loadBios("Bios.bin");
	memory.loadRom("roms/Super Mario Land (JUE) (V1.1) [!].gb");

	while (true) {
		cpu.cycle();
		input.updateKeys();
		gpu.cycle();
		memory.updateInterrupts();
		timer.update();
	}

	return 0;
}