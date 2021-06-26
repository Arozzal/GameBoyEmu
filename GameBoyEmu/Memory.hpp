#pragma once
#include "Gpu.hpp"
#include "Register.hpp"
#include "IOMap.hpp"
#include "Cpu.hpp"
#include "Input.hpp"
#include "Timer.hpp"
#include <string>

typedef unsigned char Byte;
typedef unsigned short Word;


class Memory
{
	Byte bios[0x100];
	Byte*  rom;
	Byte eram[0x8000];
	Byte wram[0x4000];
	Byte zram[0x80];
	
	Gpu* gpu;
	Cpu* cpu;
	Registers* reg;
	IOMap* io;
	Input* input;
	Timer* timer;

	struct MBC {
		Byte rombank;
		Byte rambank;
		bool ramon;
		Byte mode;
	}mbc;

	int romoffset;
	int ramoffset;
	Byte carttype;


	bool biosActive = false;

public:
	void init();
	void loadBios(const std::string& filename);
	void loadRom(const std::string& filename);
	Byte readByte(Word address);
	Word readWord(Word address);
	void writeByte(Word address, Byte byte);
	void writeWord(Word address, Word word);
	~Memory();

	void setGpu(Gpu* gpu) { this->gpu = gpu; }
	void setReg(Registers* reg) { this->reg = reg; }
	void setIoMap(IOMap* io) { this->io = io; }
	void setInput(Input* input) { this->input = input; }
	void setCpu(Cpu* cpu) { this->cpu = cpu; }
	void setTimer(Timer* timer) { this->timer = timer; }

	void updateInterrupts();
	void updateLCDGS(int mode);
	friend class Cpu;
};

