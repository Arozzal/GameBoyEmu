#include "Cpu.hpp"
#include "OpCodes.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

void Cpu::init()
{
	reg->AF = 0;
	reg->BC = 0;
	reg->DE = 0;
	reg->HL = 0;

	reg->pc = 0x100;
	reg->sp = 0;

	reg->FZ = 0;
	reg->FH = 0;
	reg->FC = 0;
	reg->FN = 0;

	t = 0;
	m = 0;
	reg->t = 0;
	reg->m = 0;
	haltStates = 0;
}

void Cpu::postOpCode(Word adress, Byte opcode)
{


	if (adress < 0xFFF) {
		std::cout << 0;
		if (adress < 0xFF) {
			std::cout << 0;
			if (adress < 0xF) {
				std::cout << 0;
			}
		}
	}

	std::cout << adress << " " << opcode_names[opcode] << " " << std::uppercase << std::hex << +opcode << '\n';
	if (opcode == 0xCB)
		std::cout << " CB: " << +mem->readByte(adress + 1) << '\n';

	debugstring << std::uppercase << std::hex << +opcode << std::endl;

	debuglist.insert(opcode_names[opcode] + " " + debugstring.str());

	debugstring = std::stringstream();
	//std::cout << std::uppercase << std::hex << +opcode << ' ' << opcode_names[opcode] << '\n';

}

void Cpu::postLastCodes()
{
	/*if (mem->biosActive)
		return;*/

	for (int i = 300; i > 0; i--) {

		if (debugvec[debugvec.size() - i] < 0xFFF) {
			std::cout << 0;
			if (debugvec[debugvec.size() - i] < 0xFF) {
				std::cout << 0;
				if (debugvec[debugvec.size() - i] < 0xF) {
					std::cout << 0;
				}
			}
		}
		std::cout << std::uppercase << std::hex << debugvec[debugvec.size() - i] << " ";
		std::cout << opcode_names[ mem->readByte(debugvec[debugvec.size() - i])];
		std::cout << " " << '\t' << std::uppercase << std::hex << +mem->readByte(debugvec[debugvec.size() - i]) << std::endl;

	}

	if (opcode == 0xCB) {
		std::cout << "CB: " << std::uppercase << std::hex << +mem->readByte(reg->pc + 1) << std::endl;
	}

	std::cout << '\n' << "-----------------------------------------------------------------------------------------------------------------------" << '\n' << '\n';



	__debugbreak();
}

void Cpu::saveOpcodes()
{
	std::ofstream fs;
	fs.open("debug.log");
	for (auto it : debuglist) {
		fs << it << '\n';
	}
	fs.close();
}

void Cpu::cycle()
{


	opcode = mem->readByte(reg->pc);

	debugvec.push_back(reg->pc);

	/*if(input->debugEnable())
    postOpCode(reg->pc, opcode);*/

	
	if (reg->pc == 0x38 && mem->biosActive == false) {
		postLastCodes();
		__debugbreak();
	}

	if (reg->pc == 0xff04)
		__debugbreak();

	/*if (reg->pc == 0x2afe)
		__debugbreak();*/

	if (input->debugSaveing())
		saveOpcodes();

	reg->t += t;
	reg->m += m;
	reg->ct = t;
	reg->cm = m;
	t = 0;
	m = 0;
	switch (opcode) {
	case 0x00:

		if(reg->pc != 0x100)
		postLastCodes();

		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x01:
		reg->BC = mem->readWord(reg->pc + 1);
		reg->pc += 3;
		t = 12;
		m = 3;
		break;
	case 0x02:
		mem->writeByte(reg->BC, reg->A);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x03:
		reg->BC++;
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x04:
		INC(reg->B);
		reg->pc++;
		break;
	case 0x05:
		DEC(reg->B);
		reg->pc++;
		break;
	case 0x06:
		reg->B = mem->readByte(reg->pc + 1);
		reg->pc += 2;
		t = 8;
		m = 2;
		break;
	case 0x07:
		RLC(reg->A);
		reg->FZ = 0;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x08:
		mem->writeWord(mem->readWord(reg->pc + 1), reg->sp);
		reg->pc+=3;
		t = 20;
		m = 5;
		break;
	case 0x09:
		ADDHL(reg->BC);
		reg->pc++;
		break;
	case 0x0A:
		reg->A = mem->readByte(reg->BC);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x0B:
		reg->BC--;
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x0C:
		INC(reg->C);
		reg->pc++;
		break;
	case 0x0D:
		DEC(reg->C);
		reg->pc++;
		break;
	
	case 0x0E:
		reg->C = mem->readByte(reg->pc + 1);
		reg->pc += 2;
		t = 8;
		m = 2;
		break;
	case 0x0F:
		RRC(reg->A);
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x11:
		reg->DE = mem->readWord(reg->pc + 1);
		reg->pc += 3;
		t = 12;
		m = 3;
		break;
	case 0x12:
		mem->writeByte(reg->DE, reg->A);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x13:
		reg->DE++;
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x14:
		INC(reg->D);
		reg->pc++;
		break;
	case 0x15:
		DEC(reg->D);
		reg->pc++;
		break;
	case 0x16:
		reg->D = mem->readByte(reg->pc + 1);
		reg->pc += 2;
		t = 8;
		m = 2;
		break;
	case 0x17:
		RL(reg->A);
		reg->pc++;
		reg->FZ = 0;
		break;
	case 0x18:{

		//unsafe!!!

		int temp = getSigned(reg->pc + 1);
		reg->pc += temp;
		t = 12;
		m = 3;
		}
		break;
	case 0x19:
		ADDHL(reg->DE);
		reg->pc++;
		break;
	case 0x1A:
		reg->A = mem->readByte(reg->DE);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x1B:
		reg->DE--;
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x1C:
		INC(reg->E);
		reg->pc++;
		break;
	case 0x1D:
		DEC(reg->E);
		reg->pc++;
		break;
	case 0x1E:
		reg->E = mem->readByte(reg->pc + 1);
		reg->pc += 2;
		t = 8;
		m = 2;
		break;
	case 0x1F:
		RR(reg->A);
		reg->FZ = 0;
		reg->pc++;
		break;
	case 0x20: {
		int value = getSigned(reg->pc + 1);

			if (reg->FZ == false)
				reg->pc += value;
			else
				reg->pc += 2;
		}
		t = 8;
		m = 2;
		break;
	case 0x21:
		

		reg->HL = mem->readWord(reg->pc + 1);
		reg->pc += 3;
		t = 12;
		m = 3;
		break;
	case 0x22:
		mem->writeByte(reg->HL, reg->A);
		reg->HL++;
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x23:
		reg->HL++;
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x24:
		INC(reg->H);
		reg->pc++;
		break;
	case 0x25:
		DEC(reg->H);
		reg->pc++;
		break;
	case 0x26:
		reg->H = mem->readByte(reg->pc + 1);
		reg->pc+=2;
		break;
	case 0x27:
		DAA();
		reg->pc++;
		break;
	case 0x28:
		{
		int value = getSigned(reg->pc + 1);

		if (reg->FZ == true)
			reg->pc += value;
		else
			reg->pc += 2;
		}
		t = 8;
		m = 2;
		break;
	case 0x29:
		ADDHL(reg->HL);
		reg->pc++;
		break;
	case 0x2A:
		reg->A = mem->readByte(reg->HL);
		reg->HL++;
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x2B:
		reg->HL--;
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x2C:
		INC(reg->L);
		reg->pc++;
		break;
	case 0x2D:
		DEC(reg->L);
		reg->pc++;
		break;
	case 0x2E:
		reg->L = mem->readByte(reg->pc + 1);
		reg->pc += 2;
		t = 8;
		m = 2;
		break;
	case 0x2F:

		reg->A =~reg->A;
		reg->FN = 1;
		reg->FH = 1;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x30:
		{
		//dangerous
		int value = getSigned(reg->pc + 1);

		if (reg->FC == false)
			reg->pc += value;
		else
			reg->pc += 2;
		}
		t = 12;
		m = 3;
		break;
	case 0x31:
		reg->sp = mem->readWord(reg->pc + 1);
		reg->pc += 3;
		t = 12;
		m = 3;
		break;
	case 0x32:
		mem->writeByte(reg->HL, reg->A);
		reg->HL--;
		reg->pc ++;
		t = 8;
		m = 2;
		break;
	case 0x34:
		{
		Byte temp = mem->readByte(reg->HL);
		INC(temp);
		mem->writeByte(reg->HL, temp);
		reg->pc++;
		t = 12;
		m = 3;
		}
		break;
	case 0x35:
		{
		Byte temp = mem->readByte(reg->HL);
		DEC(temp);//unsafe
		mem->writeByte(reg->HL, temp);
		reg->pc++;
		t = 12;
		m = 3;
		}
		break;
	case 0x36:
		mem->writeByte(reg->HL, mem->readByte(reg->pc + 1));
		reg->pc += 2;
		t = 12;
		m = 3;
		break;
	case 0x37:
		reg->FC = 1;
		reg->FH = 0;
		reg->FN = 0;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x38:
		{
		int value = getSigned(reg->pc + 1);

		if (reg->FC == true)
			reg->pc += value;
		else
			reg->pc += 2;
		}
		t = 8;
		m = 2;
		break;
	case 0x39:
		ADDHL(reg->sp);
		reg->pc++;
		break;
	case 0x3A:
		reg->A = mem->readByte(reg->HL);
		reg->HL--;
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x3C:
		INC(reg->A);
		reg->pc++;
		break;
	case 0x3D:
		DEC(reg->A);
		reg->pc++;
		break;
	case 0x3E:
		reg->A = mem->readByte(reg->pc + 1);
		reg->pc += 2;
		t = 8;
		m = 2;
		break;
	case 0x3F:
		reg->FC ^= true;
		reg->FN = 0;
		reg->FH = 0;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x40:
		reg->B = reg->B;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x42:
		reg->B = reg->D;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x44:
		reg->B = reg->H;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x46:
		reg->B = mem->readByte(reg->HL);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x47:
		reg->B = reg->A;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x4D:
		reg->C = reg->L;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x4E:
		reg->C = mem->readByte(reg->HL);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x4F:
		reg->C = reg->A;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x54:
		reg->D = reg->H;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x55:
		reg->D = reg->L;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x56:
		reg->D = mem->readByte(reg->HL);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x57:
		reg->D = reg->A;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x58:
		reg->E = reg->B;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x5D:
		reg->E = reg->L;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x5E:
		reg->E = mem->readByte(reg->HL);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x5F:
		reg->E = reg->A;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x60:
		reg->H = reg->B;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x61:
		reg->H = reg->C;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x62:
		reg->H = reg->D;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x66:
		reg->H = mem->readByte(reg->HL);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x67:
		reg->H = reg->A;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x69:
		reg->L = reg->C;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x6B:
		reg->L = reg->E;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x6E:
		reg->L = mem->readByte(reg->HL);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x6F:
		reg->L = reg->A;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x70:
		mem->writeByte(reg->HL, reg->B);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x71:
		mem->writeByte(reg->HL, reg->C);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x72:
		mem->writeByte(reg->HL, reg->D);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x73:
		mem->writeByte(reg->HL, reg->E);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x76:
		if (haltStates == 0)
			haltStates = 1;

		if (haltStates == 2) {
			haltStates = 0;
			reg->pc++;
		}
		t = 4;
		m = 1;
		break;
	case 0x77:
		mem->writeByte(reg->HL, reg->A);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x78:
		reg->A = reg->B;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x79:
		reg->A = reg->C;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x7A:
		reg->A = reg->D;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x7B:
		reg->A = reg->E;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x7C:
		reg->A = reg->H;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x7D:
		reg->A = reg->L;
		reg->pc++;
		t = 4;
		m = 1;
		break;
	case 0x7E:
	/*	if (reg->pc == 0x2b25 && reg->HL > 0x3000)
			postLastCodes();*/

		reg->A = mem->readByte(reg->HL);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x80:
		ADD(reg->B);
		reg->pc++;
		break;
	case 0x81:
		ADD(reg->C);
		reg->pc++;
		break;
	case 0x82:
		ADD(reg->D);
		reg->pc++;
		break;
	case 0x83:
		ADD(reg->E);
		reg->pc++;
		break;
	case 0x85:
		ADD(reg->L);
		reg->pc++;
		break;
	case 0x86:
		ADD(mem->readByte(reg->HL));
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x87:
		ADD(reg->A);
		reg->pc++;
		break;
	case 0x88:
		ADC(reg->B);
		reg->pc++;
		break;
	case 0x89:
		ADC(reg->C);
		reg->pc++;
		break;
	case 0x8A:
		ADC(reg->D);
		reg->pc++;
		break;
	case 0x8C:
		ADC(reg->H);
		reg->pc++;
		break;
	case 0x8E:
		ADC(mem->readByte(reg->HL));
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0x90:
		SUB(reg->B);
		reg->pc++;
		break;
	case 0x91:
		SUB(reg->C);
		reg->pc++;
		break;
	case 0x92:
		SUB(reg->D);
		reg->pc++;
		break;
	case 0x93:
		SUB(reg->E);
		reg->pc++;
		break;
	case 0x95:
		SUB(reg->L);
		reg->pc++;
		break;
	case 0x96:
		SUB(mem->readByte(reg->HL));
		reg->pc++;
		break;
	case 0x97:
		SUB(reg->A);
		reg->pc++;
		break;
	case 0x98:
		SBC(reg->B);
		reg->pc++;
		break;
	case 0x99:
		SBC(reg->C);
		reg->pc++;
		break;
	case 0x9C:
		SBC(reg->H);
		reg->pc++;
		break;
	case 0x9F:
		SBC(reg->A);
		reg->pc++;
		break;
	case 0xA0:
		AND(reg->B);
		reg->pc++;
		break;
	case 0xA1:
		AND(reg->C);
		reg->pc++;
		break;
	case 0xA2:
		AND(reg->D);
		reg->pc++;
		break;
	case 0xA3:
		AND(reg->E);
		reg->pc++;
		break;
	case 0xA7:
		AND (reg->A);
		reg->pc++;
		break;
	case 0xA8:
		XOR(reg->B);
		reg->pc++;
		break;
	case 0xA9:
		XOR(reg->C);
		reg->pc++;
		break;
	case 0xAA:
		XOR(reg->D);
		reg->pc++;
		break;
	case 0xAB:
		XOR(reg->E);
		reg->pc++;
		break;
	case 0xAE:
		XOR(mem->readByte(reg->HL));
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0xAF:
		XOR(reg->A);
		reg->pc++;
		break;
	case 0xB0:
		OR(reg->B);
		reg->pc++;
		break;
	case 0xB1:
		OR(reg->C);
		reg->pc++;
		break;
	case 0xB2:
		OR(reg->D);
		reg->pc++;
		break;
	case 0xB3:
		OR(reg->E);
		reg->pc++;
		break;
	case 0xB4:
		OR(reg->H);
		reg->pc++;
		break;
	case 0xB5:
		OR(reg->L);
		reg->pc++;
		break;
	case 0xB7:
		OR(reg->A);
		reg->pc++;
		break;
	case 0xB8:
		CP(reg->B);
		reg->pc++;
		break;
	case 0xB9:
		CP(reg->C);
		reg->pc++;
		break;
	case 0xBA:
		CP(reg->D);
		reg->pc++;
		break;
	case 0xBB:
		CP(reg->E);
		reg->pc++;
		break;
	case 0xBE:
		CP(mem->readByte(reg->HL));
		reg->pc++;
		t = 8;
		break;
	case 0xC0:
		if (reg->FZ == false)
			popStack(reg->pc);
		else
			reg->pc++;

		t = 8;
		m = 2;
		break;
	case 0xC1:
		popStack(reg->BC);
		reg->pc++;
		break;
	case 0xC2:
		if (reg->FZ == false)
			reg->pc = mem->readWord(reg->pc + 1);
		else
			reg->pc += 3;

		t = 12;
		m = 3;
		break;
	case 0xC3:
		reg->pc = mem->readWord(reg->pc + 1);
		t = 16;
		m = 4;
		break;
	case 0xC4:
		if (reg->FZ == false)
			call();
		else
			reg->pc += 3;
		break;
	case 0xC5:
		pushStack(reg->BC);
		reg->pc++;
		break;
	case 0xC6:
		ADD(mem->readByte(reg->pc + 1));
		reg->pc += 2;
		t = 8;
		m = 2;
		break;
	case 0xC7:
		reg->pc++;
		pushStack(reg->pc);
		reg->pc = 0x0000;
		break;
	case 0xC8:
		if (reg->FZ == true)
			popStack(reg->pc);
		else
			reg->pc++;

		t = 8;
		m = 2;
		break;
	case 0xC9:
		popStack(reg->pc);
		break;
	case 0xCA:
		if (reg->FZ == true)
			reg->pc = mem->readWord(reg->pc + 1);
		else
			reg->pc += 3;

		t = 12;
		m = 3;
		break;
	case 0xCB:
		reg->pc++;
		cycleCB(mem->readByte(reg->pc));
		break;
	case 0xCC:
		if (reg->FZ == true)
			call();
		else
			reg->pc += 3;

		break;
	case 0xCD:
		call();
		break;
	case 0xCE:
		ADC(mem->readByte(reg->pc + 1));
		reg->pc += 2;
		t = 8;
		m = 2;
		break;
	case 0xD0:
		if (reg->FC == false)
			popStack(reg->pc);
		else
			reg->pc++;

		t = 8;
		m = 2;
		break;
	case 0xD1:
		popStack(reg->DE);
		reg->pc++;
		break;
	case 0xD2:
		if (reg->FC == false)
			reg->pc = mem->readWord(reg->pc + 1);
		else
			reg->pc += 3;

		t = 12;
		m = 3;
		break;
	case 0xD4:
		if (reg->FC == false)
			call();
		else
			reg->pc += 3;
		break;
	case 0xD5:
		pushStack(reg->DE);
		reg->pc++;
		break;
	case 0xD6:
		SUB(mem->readByte(reg->pc + 1));
		reg->pc += 2;
		t = 8;
		m = 2;
		break;
	case 0xD8:
		if (reg->FC == true)
			popStack(reg->pc);
		else
			reg->pc++;

		t = 8;
		m = 2;
		break;
	case 0xD9:
		reti();
		break;
	case 0xDA:
		if (reg->FC == true)
			reg->pc = mem->readWord(reg->pc + 1);
		else
			reg->pc += 3;

		t = 12;
		m = 3;
		break;
	case 0xDE:
		SBC(mem->readByte(reg->pc + 1));
		reg->pc+=2;
		break;
	case 0xDF:
		reg->pc++;
		pushStack(reg->pc);
		reg->pc = 0x0018;
		break;
	case 0xE0:

		mem->writeByte(0xFF00 + mem->readByte(reg->pc + 1), reg->A);
		reg->pc += 2;
		t = 12;
		m = 3;
		break;
	case 0xE1:
		popStack(reg->HL);
		reg->pc++;
		break;
	case 0xE2:
		mem->writeByte(0xFF00 + reg->C, reg->A);
		reg->pc += 1;
		t = 8;
		m = 2;
		break;
	case 0xE5:
		pushStack(reg->HL);
		reg->pc++;
		break;
	case 0xE6:
		AND(mem->readByte(reg->pc + 1));
		reg->pc += 2;
		t = 8;
		m = 2;
		break;
	case 0xE7:
		reg->pc++;
		pushStack(reg->pc);
		reg->pc = 0x0020;
		break;
	case 0xE8:
		{
		///////////////////////////////////////////
		//VERY DANGEROUS///////////////////////////
		///////////////////////////////////////////
		char temp = getSigned(reg->pc + 1);	 //////
											 //////
			temp -= 2;						 //////
		///////////////////////////////////////////
		//VERY DANGEROUS///////////////////////////
		///////////////////////////////////////////

		reg->FN = 0;
		reg->FZ = 0;
		reg->sp += temp;

		if (reg->sp & 0xFFFF0000)
			reg->FC = 1;
		else
			reg->FC = 0;

		if (reg->sp > 0x00FF)
			reg->FH = 1;
		else
			reg->FH = 0;

		reg->pc += 2;
		t = 16;
		m = 4;
		}
		break;
	case 0xE9:		
		reg->pc = reg->HL;
		t = 4;
		m = 1;
		break;
	case 0xEA:
		mem->writeByte(mem->readWord(reg->pc + 1), reg->A);
		reg->pc += 3;
		t = 16;
		m = 4;
		break;
	case 0xEE:
 		XOR(mem->readByte(reg->pc + 1));
		reg->pc+=2;
		t = 8;
		m = 2;
		break;
	case 0xEF:
		reg->pc++;
		pushStack(reg->pc);
		reg->pc = 0x0028;
		break;
	case 0xF0:
		reg->A = mem->readByte(0xFF00 + mem->readByte(reg->pc + 1));
		reg->pc += 2;
		t = 12;
		m = 3;
		break;
	case 0xF1:
		popStack(reg->AF);
		//DANGEROUS!!!!
		//reg->F &= 0xF0;
		reg->pc++;
		break;
	case 0xF2:
		reg->A = mem->readByte(0xFF00 + reg->C);
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0xF3:
		//make interupt disable!!!!
		mem->io->intEnabled = false;
		reg->pc++;
		reg->m = 1;
		reg->t = 4;
		break;
	case 0xF5:
		pushStack(reg->AF);
		reg->pc++;
		break;
	case 0xF6:
		OR(mem->readByte(reg->pc + 1));
		reg->pc += 2;
		break;
	case 0xF9:
		reg->sp = reg->HL;
		reg->pc++;
		t = 8;
		m = 2;
		break;
	case 0xFA:
		reg->A = mem->readByte(mem->readWord(reg->pc + 1));
		reg->pc += 3;
		t = 16;
		m = 4;
		break;
	case 0xFB:
		mem->io->intEnabled = true;
		//make interupt enable!!!!
		reg->pc++;
		reg->m = 1;
		reg->t = 4;
		break;
	case 0xFE:
		CP(mem->readByte(reg->pc + 1));	
		reg->pc += 2;
		if(input->debugEnable())
		std::cout << +mem->readByte(0xff44) << std::endl;
		t = 8;
		m = 2;
		break; 
	case 0xFF:
		reg->pc++;
		pushStack(reg->pc);
		reg->pc = 0x0038;
		break;
	default:
		postLastCodes();
		__debugbreak();
		break;
	}
}

//////////////
//CB OpCodes//
//////////////

void Cpu::cycleCB(Byte opcode)
{
	switch (opcode) {
	case 0x00:
		RLC(reg->B);
		reg->pc++;
		break;
	case 0x01:
		RLC(reg->C);
		reg->pc++;
		break;
	case 0x10:
		RL(reg->B);
		reg->pc++;
		break;
	case 0x11:
		RL(reg->C);
		reg->pc++;
		break;
	case 0x12:
		RL(reg->D);
		reg->pc++;
		break;
	case 0x13:
		RL(reg->E);
		reg->pc++;
		break;
	case 0x18:
		RR(reg->B);
		reg->pc++;
		break;
	case 0x19:
		RR(reg->C);
		reg->pc++;
		break;
	case 0x23:
		SLA(reg->E);
		reg->pc++;
		break;
	case 0x27:
		SLA(reg->A);
		reg->pc++;
		break;
	case 0x33:
		SWAP(reg->E);
		reg->pc++;
		break;
	case 0x37:
		SWAP(reg->A);
		reg->pc++;
		break;
	case 0x39:
		SRL(reg->C);
		reg->pc++;
		break;
	case 0x3C:
		SRL(reg->H);
		reg->pc++;
		break;
	case 0x3D:
		SRL(reg->L);
		reg->pc++;
		break;
	case 0x3F:
		SRL(reg->A);
		reg->pc++;
		break;
	case 0x40:
		BIT(0, reg->B);
		reg->pc++;
		break;
	case 0x41:
		BIT(0, reg->C);
		reg->pc++;
		break;
	case 0x43:
		BIT(0, reg->E);
		reg->pc++;
		break;
	case 0x46:
		BIT(0, mem->readByte(reg->HL));
		t = 16;
		m = 4;
		reg->pc++;
		break;
	case 0x47:
		BIT(0, reg->A);
		reg->pc++;
		break;
	case 0x48:
		BIT(1, reg->B);
		reg->pc++;
		break;
	case 0x4E:
		BIT(1, mem->readByte(reg->HL));
		reg->pc++;
		t = 16;
		m = 4;
		break;
	case 0x50:
		BIT(2, reg->B);
		reg->pc++;
		break;
	case 0x57:
		BIT(2, reg->A);
		reg->pc++;
		break;
	case 0x58:
		BIT(3, reg->B);
		reg->pc++;
		break;
	case 0x5E:
		BIT(3, mem->readByte(reg->HL));
		reg->pc++;
		t = 16;
		m = 4;
		break;
	case 0x5F:
		BIT(3, reg->A);
		reg->pc++;
		break;
	case 0x60:
		BIT(4, reg->B);
		reg->pc++;
		break;
	case 0x61:
		BIT(4, reg->C);
		reg->pc++;
		break;
	case 0x67:
		BIT(4, reg->A);
		reg->pc++;
		break;
	case 0x68:
		BIT(5, reg->B);
		reg->pc++;
		break;
	case 0x69:
		BIT(5, reg->C);
		reg->pc++;
		break;
	case 0x6E:
		BIT(5, mem->readByte(reg->HL));
		reg->pc++;
		t = 16;
		m = 2;
		break;
	case 0x6F:
		BIT(5, reg->A);
		reg->pc++;
		break;
	case 0x70:
		BIT(6, reg->B);
		reg->pc++;
		break;
	case 0x71:
		BIT(6, reg->C);
		reg->pc++;
		break;
	case 0x76:
		BIT(6, mem->readByte(reg->HL));
		reg->pc++;
		t = 16;
		m = 2;
		break;
	case 0x77:
		BIT(6, reg->A);
		reg->pc++;
		break;
	case 0x78:
		BIT(7, reg->B);
		reg->pc++;
		break;
	case 0x79:
		BIT(7, reg->C);
		reg->pc++;
		break;
	case 0x7C:
		BIT(7, reg->H);
		reg->pc++;
		break;
	case 0x7E:
		BIT(7, mem->readByte(reg->HL));
		reg->pc++;
		t = 16;
		m = 2;
		break;
	case 0x7F:
		BIT(7, reg->A);
		reg->pc++;
		break;
	case 0x86:
		{
		Byte temp = mem->readByte(reg->HL);
		RES(0, temp);
		mem->writeByte(reg->HL, temp);
		reg->pc++;
		t = 16;
		m = 4;
		}
		break;
	case 0x87:
		RES(0, reg->A);
		reg->pc++;
		break;
	case 0x8E:
		RESMEM(1, reg->HL);
		reg->pc++;
		break;
	case 0x96:
		RESMEM(2, reg->HL);
		reg->pc++;
		break;
	case 0x9E:
		{
		Byte temp = mem->readByte(reg->HL);
		RES(3, temp);
		mem->writeByte(reg->HL, temp);
		reg->pc++;
		t = 16;
		m = 4;
		}
		break;
	case 0x9F:
		RES(3, reg->A);
		reg->pc++;
		break;
	case 0xB6:
		RESMEM(6, reg->HL);
		reg->pc++;
		break;
	case 0xB9:
		RES(7, reg->C);
		reg->pc++;
		break;
	case 0xBE:
		{
		Byte temp = mem->readByte(reg->HL);
		RES(7, temp);
		mem->writeByte(reg->HL, temp);
		reg->pc++;
		t = 16;
		m = 4;
		}
		break;
	case 0xC3:
		SET(0, reg->E);
		reg->pc++;
		break;
	case 0xC6:
		SETMEM(0, reg->HL);
		reg->pc++;
		break;
	case 0xD0:
		SET(2, reg->B);
		reg->pc++;
		break;
	case 0xD8:
		SET(3, reg->B);
		reg->pc++;
		break;
	case 0xDE:
		{
		Byte temp = mem->readByte(reg->HL);
		SET(3, temp);
		mem->writeByte(reg->HL, temp);
		reg->pc++;
		t = 16;
		m = 4;
		}
		break;
	case 0xF0:
		SET(6, reg->B);
		reg->pc++;
		break;
	case 0xF6:
		SETMEM(6, reg->HL);
		reg->pc++;
		break;
	case 0xF8:
		SET(7, reg->B);
		reg->pc++;
		break;
	case 0xFE:
		{
		Byte temp = mem->readByte(reg->HL);
		SET(7, temp);
		mem->writeByte(reg->HL, temp);
		reg->pc++;
		t = 16;
		m = 4;
		}
		break;
	default:
		reg->pc--;
		postLastCodes();
		postOpCode(reg->pc, opcode);
		__debugbreak();
		break;
	}
}

////////////////////
//Assembly Methods//
////////////////////


void Cpu::SWAP(Byte & byte)
{
	Byte temp = byte & 0xF0;
	temp >>= 4;
	byte <<= 4;
	byte |= temp;
	reg->FZ = (byte == 0 ? 1 : 0);
	reg->FN = 0;
	reg->FH = 0;
	reg->FC = 0;
	t += 8;
	m += 2;
}

void Cpu::SUB(Byte byte)
{
	if ((((reg->A & 0xF) - (byte & 0xF) & 0x10)) == 0x10)
		reg->FH = true;
	else
		reg->FH = false;

	if ((reg->A - byte) < 0)
		reg->FC = true;
	else
		reg->FC = false;

	reg->A -= byte;
	reg->FN = true;
	reg->FZ = (reg->A == 0 ? 1 : 0);
	t += 4;
	m += 1;
}

void Cpu::SBC(Byte byte)
{

	if ((reg->A & 0x0F) < ((byte + reg->FC) & 0x0F))
		reg->FH = true;
	else
		reg->FH = false;


	if ((reg->A - (byte + reg->FC)) < 0)
		reg->FC = true;
	else
		reg->FC = false;

	reg->FN = true;
	
	reg->A -= (byte + reg->FC);

	reg->FZ = (reg->A == 0 ? 1 : 0);

	t += 4;
	m += 1;
}

void Cpu::ADD(Byte  byte)
{
	reg->F = 0;

	reg->FN = 0;
	if ((((reg->A & 0xF) + (byte & 0xF)) & 0x10) == 0x10)
		reg->FH = true;

	if ((reg->A + byte) > 255)
		reg->FC = true;

	reg->A += byte;

	if (reg->A == 0)
		reg->FZ = true;


	t += 4;
	m += 1;
}

void Cpu::ADC(Byte byte)
{
	if ((((reg->A & 0xF) + ((byte + reg->FC) & 0xF)) & 0x10) == 0x10)
		reg->FH = true;
	else
		reg->FH = false;

	reg->FN = 0;
	reg->A += (byte + reg->FC);
	
	if ((reg->A + byte + reg->FC) > 0xFF)
		reg->FC = true;
	else
		reg->FC = false;

	reg->FZ = (reg->A == 0 ? 1 : 0);
	t += 4;
	m += 1;
}

void Cpu::ADDHL(Word word)
{
	if ((((word & 0x0FFF) + (reg->HL & 0x0FFF)) & 0x1000) == 0x1000)
		reg->FH = true;
	else
		reg->FH = false;

	reg->FN = 0;
	if ((word + reg->HL) > 0xFFFF)
		reg->FC = true;
	else
		reg->FC = false;
		
	reg->HL += word;
	t += 8;
	m += 2;
}

void Cpu::DEC(Byte & byte)
{
	if ((((byte & 0xF) - (1 & 0xF) & 0x10)) == 0x10)
		reg->FH = true;
	else
		reg->FH = false;
		
	byte--;
	
	reg->FN = true;
	reg->FZ = (byte == 0 ? 1 : 0);
	t += 4;
	m += 1;
}

void Cpu::INC(Byte & byte)
{
	if (((byte & 0xF) + (1 & 0xF) & 0x10) == 0x10)
		reg->FH = true;
	else
		reg->FH = false;
		
	byte++;
	reg->FN = false;
	reg->FZ = (byte == 0 ? 1 : 0);
	t += 4;
	m += 1;
}

void Cpu::RL(Byte & byte)
{
	bool carry = reg->FC;
	
	reg->FC = (byte >> 7);

	byte <<= 1;
	byte |= carry;


	reg->FZ = (reg->C == 0 ? 1 : 0);
	reg->FN = 0;
	reg->FH = 0;

	t += 8;
	m += 2;
}

void Cpu::RLC(Byte & byte)
{
	//DANGEROUSR!!!!!!!!!!!!!!!!!
	reg->FC = ((byte & 0x80) >> 7);
	byte <<= 1;
	byte |= reg->FC;

	reg->FZ = (byte == 0 ? 1 : 0);
	reg->FN = 0;
	reg->FH = 0;

	t += 8;
	m += 2;
}

void Cpu::RR(Byte & byte)
{
	Byte temp = reg->FC;
	reg->FC = byte & 1;
	byte >>= 1;
	byte |= (temp << 7);

	reg->FN = 0;
	reg->FH = 0;
	reg->FZ = (byte == 0 ? 1 : 0);

	t += 8;
	m += 2;
}

void Cpu::RRC(Byte & byte)
{
	reg->FC = byte & 1;
	byte >>= 1;
	byte |= (reg->FC << 7);

	reg->FN = 0;
	reg->FH = 0;
	reg->FZ = (byte == 0 ? 1 : 0);

	t += 8;
	m += 2;
}

void Cpu::CP(Byte byte)
{
	reg->F = 0;

	if (((reg->A & 0xF) - (byte & 0xF) & 0x10))
		reg->FH = true;

	if (reg->A < byte)
		reg->FC = true;

	if (reg->A == byte)
		reg->FZ = 1;

	reg->FN = 1;

	t += 4;
	m += 1;
}

void Cpu::OR(Byte byte)
{
	reg->A |= byte;
	reg->FZ = (reg->A == 0 ? 1 : 0);
	reg->FN = 0;
	reg->FH = 0;
	reg->FC = 0;
	t += 4;
	m += 1;
}

void Cpu::XOR(Byte byte)
{
	reg->A ^= byte;
	reg->FZ = (reg->A == 0 ? 1 : 0);
	reg->FN = 0;
	reg->FH = 0;
	reg->FC = 0;
	t += 4;
	m += 1;
}

void Cpu::AND(Byte byte)
{
	reg->A &= byte;
	reg->FZ = (reg->A == 0 ? 1 : 0);
	reg->FN = 0;
	reg->FH = 1;
	reg->FC = 0;
	t += 4;
	m += 1;
}

void Cpu::RES(Byte num, Byte & byte)
{
	Byte resv = ~(0x1 << num);
	byte &= resv;
	t += 8;
	m += 2;
}

void Cpu::RESMEM(Byte num, Word address)
{
	Byte temp = mem->readByte(address);
	RES(num, temp);
	mem->writeByte(address, temp);
	t += 8;
	m += 2;
}

void Cpu::SET(Byte num, Byte & byte)
{
	Byte resv = (1 << num);
	byte |= resv;
	t += 8;
	m += 2;
}

void Cpu::SETMEM(Byte num, Word address)
{
	Byte temp = mem->readByte(address);
	SET(num, temp);
	mem->writeByte(address, temp);
	t += 8;
	m += 2;
}

void Cpu::BIT(Byte num, Byte byte)
{
	Byte pos = 1;
	pos <<= num;
	reg->FZ = ((byte & pos) == 0 ? 1 : 0);
	reg->FN = 0;
	reg->FH = 1;
	t += 8;
	m += 2;
}

void Cpu::SRL(Byte & byte)
{
	//MSB not implemented!

	reg->FC = (byte & 1);

	byte >>= 1;

	reg->FN = 0;
	reg->FH = 0;

	reg->FZ = (byte == 0 ? 1 : 0);

	t += 8;
	m += 2;
}

void Cpu::SLA(Byte & byte)
{
	//LSB not implemented!

	reg->FC = ((byte & 0x80) >> 7);

	byte <<= 1;

	reg->FN = 0;
	reg->FH = 0;

	reg->FZ = (byte == 0 ? 1 : 0);

	t += 8;
	m += 2;
}

void Cpu::DAA()
{

	if (!reg->FH){
		if (reg->FH || (reg->A & 0xF) > 9)
			reg->A += 0x06;

		if (reg->FC || reg->A > 0x9F)
			reg->A += 0x60;
	}else{
		if (reg->FH)
			reg->A = (reg->A - 6) & 0xFF;

		if (reg->FC)
			reg->A -= 0x60;
	}

	reg->FH = 0;

	if ((reg->A & 0x100) == 0x100)
		reg->FC = 1;

	reg->FZ = (reg->A == 0 ? 1 : 0);

}

char Cpu::getSigned(Word address)
{
	/*int val = mem->readByte(address);

	if (val > 127)
		val -= 256;*/

	Byte value = mem->readByte(address);
	char endvalue = value;

	if (value > 127)
		endvalue = -((~value) - 1);
	else
		endvalue += 2;

	return endvalue;
}

void Cpu::reti()
{
	mem->io->intEnabled = true;
	popStack(reg->pc);

	m += 3;
	t += 12;
}


void Cpu::call()
{
	pushStack(reg->pc + 3);
	reg->pc = mem->readWord(reg->pc + 1);
}

void Cpu::pushStack(Word word)
{
	mem->writeByte(reg->sp - 1, (word & 0xFF00) >> 8);
	mem->writeByte(reg->sp - 2,  word & 0xFF);
	reg->sp -= 2;
	t += 16;
	m += 4;
}

void Cpu::popStack(Word & word)
{
	word = Word(mem->readByte(reg->sp) | (mem->readByte(reg->sp + 1) << 8));
	reg->sp += 2;
	t += 12;
	m += 3;
}


