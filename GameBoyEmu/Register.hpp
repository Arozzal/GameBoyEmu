#pragma once

typedef unsigned char Byte;
typedef unsigned short Word;

class Memory;
class Gpu;
class Cpu;

struct Registers {
	
	long long t, m;
	int ct, cm;

	union {
		Word AF;
		struct {
			union {
				Byte F;
				struct {
					bool nop1 : 1;
					bool nop2 : 1;
					bool nop3 : 1;
					bool nop4 : 1;

					bool FC : 1; //Carry Flag
					bool FH : 1; //Half Carry Flag
					bool FN : 1; //Subtract Flag
					bool FZ : 1; //Zero Flag
				};
			};
			Byte A;
		};
	};

	union {
		Word BC;
		struct {
			Byte C;
			Byte B;
		};
	};

	union {
		Word DE;
		struct {
			Byte E;
			Byte D;
		};
	};

	union {
		Word HL;
		struct {
			Byte L;
			Byte H;
		};
	};

	

	Word sp;
	Word pc;
};