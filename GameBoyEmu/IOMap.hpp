#pragma once
#include "Register.hpp"

struct IOMap {

	//0xFF40

	union LCDGPUcontroll {
		Byte LCDG;

		struct {
			Byte bgOn : 1;
			Byte spriteOn : 1;
			Byte spriteSize : 1;
			Byte bgTileMap : 1;
			Byte bgTileSet : 1;
			Byte windowOn : 1;
			Byte tileMap : 1;
			Byte displayOn : 1;
		};

	}LCDG;


	Byte scrollY;
	Byte scrollX;
	Byte currentScanLine;
	Byte bgpallet;

	//0xFF41

	union LCDGStatus {
		Byte LCDGS;

		struct {
			Byte mode1 : 1;
			Byte mode2 : 1;
			Byte CoincidenceFlag : 1;
			Byte HBlankInt : 1;
			Byte VBlankInt : 1;
			Byte OamInt : 1;
			Byte LYC : 1;
		};

		Byte LCDGStatus::getMode() {
			return (mode2 << 1 | mode1);
		}

		void LCDGStatus::setMode(Byte mode) {
			mode2 = (mode & 2) != 0;
			mode1 = (mode & 1) != 0;
		}

	}LCDGS;

	Byte LYC;


	//Interrupts

	union Interrupts {
		Byte intr;
		struct {
			bool vblank : 1;
			bool lcdStat : 1;
			bool timer : 1;
			bool serial : 1;
			bool joypad : 1;
		};
	};


	bool intEnabled;
	Interrupts intE;
	Interrupts intF;
};