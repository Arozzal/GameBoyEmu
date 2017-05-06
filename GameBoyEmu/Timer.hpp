#pragma once
#include "Register.hpp"
#include "IOMap.hpp"

class Timer {
	Registers* reg;
	IOMap* io;
	Word div, sub, tmain;


	Byte rdiv;
	Byte rcoun;
	Word rmod;
	Byte rcon;


public:

	Timer() {
		rdiv = 0;
		rcoun = 0;
		rmod = 0;
		rcon = 0;

		div = 0; 
		sub = 0;
		tmain = 0;
	}


	void Timer::update() {
		sub += reg->cm;

		if (sub >= 4) {
			tmain++;
			sub -= 4;

			div++;
			if (div == 16) {
				rdiv = ((rdiv + 1) & 0xFF);
				div = 0;
			}

		}

		Byte tichigh;
		switch (rcon & 3) {
		case 0: tichigh = 64; break;
		case 1: tichigh = 1; break;
		case 2: tichigh = 4; break;
		case 3: tichigh = 16; break;
		}

		if (tmain >= tichigh) {


			tmain = 0;
			rcoun++;

			if (rcon > 255) {
				rcon = rmod;
				io->intF.timer = true;
			}

		}


	}


	void setTimeReg(Word address, Byte byte) {
		switch (address) {
		case 0xFF04: rdiv = 0 ; break;
		case 0xFF05: rcoun = byte; break;
		case 0xFF06: rmod = byte; break;
		case 0xFF07: rcon = byte & 7; 
		}
	}

	Byte getTimeReg(Word address) {
		switch (address) {
		case 0xFF04: return rdiv;
		case 0xFF05: return rcoun;
		case 0xFF06: return rmod;
		case 0xFF07: return rcon;
		default: __debugbreak(); break;
		}
	}


	void Timer::setRegisters(Registers* reg) { this->reg = reg; }
	void Timer::setIoMap(IOMap* io) { this->io = io; }
};