#include "Memory.hpp"

#include "Register.hpp"

#include <iostream>
#include <fstream>







void Memory::init()
{
	memset(gpu->vram, 0, sizeof(gpu->vram));
	memset(eram, 0, sizeof(eram));
	memset(wram, 0, sizeof(wram));
	memset(gpu->Oam, 0, sizeof(gpu->Oam));
	memset(zram, 0, sizeof(zram));
	io->bgpallet = 0;
	io->currentScanLine = 0;
	io->LCDG.LCDG = 0;
	io->scrollX = 0;
	io->scrollY = 0;
	biosActive = false;
	io->intF.intr = 0;
	io->intE.intr = 0;
	io->LCDGS.LCDGS = 0;
}

void Memory::loadBios(const std::string& filename)
{
	std::ifstream fs;
	fs.open(filename, std::ios::binary);
	if (fs.is_open()) {
		fs.seekg(0);
		fs.read((char*)bios, sizeof(bios));
	}
	else {
		std::cerr << "Memory: Unable to load Bios!" << std::endl;
	}

}

void Memory::loadRom(const std::string & filename)
{
	std::ifstream fs;
	fs.open(filename, std::ios::binary | std::ios::ate);
	if (fs.is_open()) {
		int size = fs.tellg();
		rom = new Byte[size];
		fs.seekg(0);

		fs.read((char*)rom, size);
	}
	else {
		std::cerr << "Memory: Unable to load Rom!" << std::endl;
	}
}

Byte Memory::readByte(Word address)
{


	switch (address & 0xF000) {
	case 0x0000:

		if (biosActive) {
			if(address < 0x0100)
				return bios[address];
			else if(reg->pc == 0x0100){
				biosActive = false;
			}
		}
		return rom[address];
	case 0x1000:
	case 0x2000:
	case 0x3000:

	case 0x4000: case 0x5000: case 0x6000: case 0x7000:
		return rom[address];
	case 0x8000:
	case 0x9000:
		return gpu->vram[address - 0x8000];

		break;
	case 0xA000:
	case 0xB000:
		return eram[address - 0xA000];
		break;
	case 0xC000:
	case 0xD000:
		return wram[address - 0xC000];
		break;
	case 0xE000:
		return wram[address - 0xE000];
		break;
	case 0xF000:
		switch (address & 0x0F00) {
		case 0x000: case 0x100: case 0x200: case 0x300:
		case 0x400: case 0x500: case 0x600: case 0x700:
		case 0x800: case 0x900: case 0xA00: case 0xB00:
		case 0xC00: case 0xD00:
			return wram[address - 0xE000];
			break;
		case 0xE00:
			if (address < 0xFEA0) {
				return gpu->Oam[address - 0xFE00];
			}
			return 0;
			break;
		case 0xF00:
			if (address == 0xFFFF)
				return io->intE.intr;

			if (address >= 0xFF80) {
				return zram[address - 0xFF80];
			}
			switch (address & 0x00FF) {
			case 0x00:
				return input->getKeys();
			case 0x04: case 0x05:
			case 0x06: case 0x07:
				return timer->getTimeReg(address);
			case 0x0F:
				return io->intF.intr;
			case 0x40:
				return io->LCDG.LCDG;
			case 0x41:
				return io->LCDGS.LCDGS;
			case 0x42:
				return io->scrollY;
			case 0x43:
				return io->scrollX;
			case 0x44:
				return io->currentScanLine;
			case 0x45:
				return io->LYC;
			default:
				std::cout << std::uppercase << std::hex << address << std::endl;
				if (address == 0xFF36) {
					__debugbreak();
					cpu->postLastCodes();
				}
				break;
			}

			break;
		}

	default:
		//__debugbreak();
		break;
	}

	//__debugbreak();
	return rand() % 255;
}

Word Memory::readWord(Word address)
{
	return ((readByte(address + 1) << 8) | readByte(address));
}

void Memory::writeByte(Word address , Byte byte)
{
	/*if ((address & 0xFF00) == 0xC000 && (byte > 0xa0))
		cpu->postLastCodes();*/


	/*if (byte == 0xed && address == 0xc012)
		cpu->postLastCodes();
		*/
	/*if (byte == 0xed && address >= 0xff88 && address <= 0xff93)
		cpu->postLastCodes();

		*/



	/*if (address == 0xff89)
		cpu->postLastCodes();*/

	switch (address & 0xF000) {
	case 0x0000: case 0x1000: case 0x2000:
	case 0x3000: case 0x4000: case 0x5000:
	case 0x6000: case 0x7000:
		//__debugbreak();
		break;
	case 0x8000:
	case 0x9000:
		gpu->vram[address & 0x1FFF] = byte;
		if(address < 0x9800)
			gpu->updateTileSet(address);
		break;
	case 0xA000:
	case 0xB000:
		eram[address - 0xA000] = byte;
		break;
	case 0xC000:
	case 0xD000:
		wram[address - 0xC000] = byte;
		break;
	case 0xE000:
		wram[address - 0xE000] = byte;
		break;
	case 0xF000:
		switch (address & 0x0F00) {
		case 0x000: case 0x100: case 0x200: case 0x300:
		case 0x400: case 0x500: case 0x600: case 0x700:
		case 0x800: case 0x900: case 0xA00: case 0xB00:
		case 0xC00: case 0xD00:
			wram[address - 0xE000] = byte;
			break;
		case 0xE00:
			if (address < 0xFEA0) {

			
				Byte pos = address - 0xFE00;

				
					

				gpu->Oam[pos] = byte;

				Gpu::Sprite& spr = gpu->getSprite(pos >> 2);
				switch (address & 3) {
				case 0:
					spr.Y = byte - 16;
					break;
				case 1:
					spr.X = byte - 8;
					break;
				case 2:
					/*if (byte == 0xed)
						cpu->postLastCodes();*/
					spr.num = byte;
					break;
				case 3:
					spr.options = byte;
					break;
				}

			}
			break;
		case 0xF00:
			if (address == 0xFFFF)
				io->intE.intr = byte;

			if (address >= 0xFF80) {

				zram[address - 0xFF80] = byte;
				break;
			}
			switch (address & 0x00FF) {
			case 0x00:
				input->setKeyCode(byte);
				break;
			case 0x04: case 0x05:
			case 0x06: case 0x07:
				timer->setTimeReg(address, byte);
				break;
			case 0x40:
				io->LCDG.LCDG = byte;
				break;
			case 0x41:
				io->LCDGS.LCDGS = ((byte & 0xFC) | io->LCDGS.getMode());
				break;
			case 0x42:
				io->scrollY = byte;
				break;
			case 0x43:
				io->scrollX = byte;
				break;
			case 0x45:
				io->LYC = byte;
				break;
			case 0x46:
				{
				
				Word address = byte << 8;
				for (int i = 0; i < sizeof(gpu->Oam); i++) {
					writeByte(0xFE00 + i, readByte(address + i));
				}
				}
				break;
			case 0x47:
			case 0x48:
			case 0x49:

				for (int i = 0; i < 4; i++) {
					Byte temp = ((byte >> (i * 2)) & 0x3);
					gpu->setColors((address & 0xF) - 7, i, temp);
				}
				break;
			default:
				std::cout << std::uppercase << std::hex << address << std::endl;
				break;
			}

		default:
			if (byte == 0xFF37)
				cpu->postLastCodes();

			break;
		}
	default:
		//__debugbreak();
		break;
	}

}

void Memory::writeWord(Word address, Word word)
{

	Byte b1 = (word & 0xFF00) >> 8;
	Byte b2 = (word & 0xFF);

	writeByte(address,     b2);
	writeByte(address + 1, b1);
}


Memory::~Memory()
{
	delete[] rom;
}

void Memory::updateInterrupts()
{
	if (io->currentScanLine == io->LYC)
		io->LCDGS.CoincidenceFlag = true;

	else
		io->LCDGS.CoincidenceFlag = false;


	if (io->intEnabled == false)
		return;

	if(io->intF.lcdStat && io->intE.lcdStat)
	switch (io->LCDGS.getMode()) {
	case 0:
		if (io->LCDGS.HBlankInt)
			goto code;

		break;
	case 1:
		if (io->LCDGS.VBlankInt)
			goto code;

		break;
	case 2:
		if (io->LCDGS.OamInt)
			goto code;

		break;
	case 3:
		if (io->LCDGS.LYC)
			goto code;

		break;
	code:
		io->intF.lcdStat = false;
		io->intEnabled = false;
		cpu->pushStack(reg->pc);
		reg->pc = 0x0048;
		if (cpu->haltStates == 1)
			cpu->haltStates = 2;

		break;
	default:
		__debugbreak();
		break;
	}

	if (io->intE.timer && io->intF.timer) {
		io->intEnabled = false;
		io->intF.timer = false;
		cpu->pushStack(reg->pc);
		reg->pc = 0x0050;
		if (cpu->haltStates == 1)
			cpu->haltStates = 2;
	}

	if (io->intF.vblank && io->intE.vblank) {
		io->intEnabled = false;
		io->intF.vblank = false;
		cpu->pushStack(reg->pc);
		reg->pc = 0x0040;
		if (cpu->haltStates == 1)
			cpu->haltStates = 2;
	}




}

void Memory::updateLCDGS(int mode)
{
	io->LCDGS.setMode(mode);
	switch (mode) {
	case 0:
		if (io->LCDGS.HBlankInt && io->intEnabled)
			io->intF.lcdStat = true;
		break;
	case 1:
		if (io->LCDGS.VBlankInt && io->intEnabled)
			io->intF.lcdStat = true;
		break;
	case 2:
		if (io->LCDGS.OamInt && io->intEnabled)
			io->intF.lcdStat = true;
		break;
	case 3:
		if (io->LCDGS.LYC && io->intEnabled)
			io->intF.lcdStat = true;
		break;
	}


}
