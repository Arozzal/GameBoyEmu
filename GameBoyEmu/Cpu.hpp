#pragma once
#include "Register.hpp"
#include "Memory.hpp"
#include "Input.hpp"
#include <set>
#include <sstream>
#include <vector>


class Cpu
{	
	std::set<std::string> debuglist;
	Registers* reg;
	Memory* mem;
	Input* input;
	Word opcode;
	std::stringstream debugstring;
	std::vector<Word> debugvec;
	int t, m;
	Byte haltStates;

public:
	void init();
	void cycle();

	void setRegister(Registers* reg) { this->reg = reg; }
	void setMemory(Memory* mem) { this->mem = mem; }
	void setInput(Input* input) { this->input = input; }
private:

	void cycleCB(Byte opcode);

	void postOpCode(Word adress, Byte opcode);
	void postLastCodes();

	void saveOpcodes();
	
	void SWAP(Byte& byte);
	void SUB(Byte byte);
	void SBC(Byte byte);
	void ADD(Byte byte);
	void ADC(Byte byte);
	void ADDHL(Word word);
	void DEC(Byte& byte);
	void INC(Byte& byte);
	void RL(Byte& byte);
	void RLC(Byte& byte);
	void RR(Byte& byte);
	void RRC(Byte& byte);
	void CP(Byte byte);
	void OR(Byte reg);
	void XOR(Byte byte);
	void AND(Byte byte);
	void RES(Byte num, Byte& byte);
	void RESMEM(Byte num, Word address);
	void SET(Byte num, Byte& byte);
	void SETMEM(Byte num, Word address);
	void BIT(Byte num, Byte byte);
	void SRL(Byte& byte);
	void SLA(Byte& byte);
	void DAA();
	
	char getSigned(Word address);

	void reti();

	
	void call();
	void pushStack(Word word);
	void popStack(Word& word);

	friend class Memory;
};

