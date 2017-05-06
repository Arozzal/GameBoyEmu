#pragma once
#include <SDL.h>
#include "IOMap.hpp"
#include "Register.hpp"
typedef unsigned char Byte;
typedef unsigned short Word;

class Gpu
{
public:
	struct Sprite {
		Byte Y;
		Byte X;
		Byte num;

		union {
			Byte options;
			struct {
				bool above : 1;
				bool YFlip : 1;
				bool XFlip : 1;
				bool palette : 1;
			};
		};
	};

private:

	Sprite spritearr[40];
	IOMap* io;
	Registers* reg;
	Memory* mem;

	int mode;
	int modeClock;

	Byte lastScrollX, lastScrollY;
	bool changed = false;

	Byte bgPalette[4] = {
		0xFF,
		0x92,
		0xA4,
		0x00,
	};



	Byte colors[3][4];
	
	Byte pixels[160 * 145];

	Byte tileset[511][8][8];


	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect scroll;
	SDL_Event event;
public:
	Gpu();
	void cycle();
	void draw();

	void renderScan();
	void updateTileSet(Word address);

	void setIo(IOMap* io) { this->io = io; }
	void change() { this->changed = true; }

	void setRegisters(Registers* reg) { this->reg = reg; }
	void setMemory(Memory* mem) { this->mem = mem; }
	void setColors(Byte pal, Byte id, Byte val) { colors[pal][id] = bgPalette[val]; }

	Sprite& getSprite(Byte index) { 
		return spritearr[index]; 
	}

	Byte Oam[0xA0];
	Byte vram[0x2000];
};

