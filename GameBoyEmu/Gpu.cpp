#include "Gpu.hpp"
#include "Memory.hpp"
#include <memory>

Gpu::Gpu() {
	memset(pixels, 255, 144 * 160 * sizeof(Byte));
	memset(tileset, 0, sizeof(tileset));
	window = SDL_CreateWindow("GB", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 576, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STATIC, 160, 144);
	mode = 0;
	modeClock = 0;
}

void Gpu::cycle()
{
	modeClock += reg->ct;


	switch (io->LCDGS.getMode()) {
	case 0:
		if (modeClock >= 204) {
			modeClock = 0;
			io->currentScanLine++;
			if (io->currentScanLine == 143) {
				mem->updateLCDGS(1);
				io->intF.vblank = true;
				draw();
			}
			else {
				mem->updateLCDGS(2);
			}
		}
		break;
	case 1:
		if (modeClock >= 456) {
			modeClock = 0;
			io->currentScanLine++;

			if (io->currentScanLine > 153) {
				mem->updateLCDGS(2);
				io->currentScanLine = 0;
			}
			
		}
		break;
	case 2:
		if (modeClock >= 80) {
			modeClock = 0;
			mem->updateLCDGS(3);
		}
		break;
	case 3:
		if (modeClock >= 172) {
			modeClock = 0;
			mem->updateLCDGS(0);
			renderScan();
		}
		break;
	}
	
}

void Gpu::draw()
{
	SDL_PollEvent(&event);

	SDL_UpdateTexture(texture, NULL, pixels, 160 * sizeof(Byte));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}


void Gpu::renderScan()
{
	Byte scanRow[160 * 145];
	memset(scanRow, 0, sizeof(scanRow));


	if (io->LCDG.bgOn){
		Word offset = io->LCDG.bgTileMap ? 0x1C00 : 0x1800;
		//io->scrollX = 0x90;
		offset += (((io->scrollY + io->currentScanLine) & 255 )>> 3) * 32;

		Word y = (io->scrollY + io->currentScanLine) & 7;


		Word lineoffset = (io->scrollX >> 3);

		int tile = vram[offset + lineoffset];
		Byte xval = io->scrollX & 0x7;

		if (io->LCDG.bgTileSet == 0 && tile < 128)tile += 256;

		Word dispos = io->currentScanLine * 160;

		for (int x = 0; x < 160;) {

			
			Byte temp = colors[0][tileset[tile][y][xval]];
			pixels[dispos + x] = temp;
			scanRow[dispos + x] = tileset[tile][y][xval];

			x++;
			xval++;


			/*if (offset + (x >> 3) == 0x18a7)
				int d = 0;*/

			if (xval == 8) {
				lineoffset = (lineoffset + 1) & 31;
				tile = vram[offset + lineoffset];

				if (io->LCDG.bgTileSet == 0 && tile < 128)tile += 256;
				xval = 0;
			}

		}
	}

	if (io->LCDG.spriteOn) {
		for (int i = 0; i < 40; i++) {
			Sprite& spr = spritearr[i];

			if (spr.Y <= io->currentScanLine && (spr.Y + 8) > io->currentScanLine) {
				Word offset = 160 * io->currentScanLine + spr.X;
				Word palette = 1 + spr.palette;
				Byte* sprRow;

			/*	if (spr.num >= 0xB0)
					__debugbreak();*/


				if (spr.YFlip) {
					sprRow = tileset[spr.num][7 - (io->currentScanLine - spr.Y)];
				}
				else {
					sprRow = tileset[spr.num][io->currentScanLine - spr.Y];
				}
				for (int x = 0; x < 8; x++) {
					if ((spr.X + x) >= 0 && (spr.X + x) < 160 && sprRow[spr.XFlip ? (7 - x) : x] &&
						(!spr.above || !scanRow[spr.X + x])) {

						pixels[offset + x] = colors[palette][ sprRow[spr.XFlip ? (7 - x) : x] ];


					}
				}
			}
		}
	}

}

void Gpu::updateTileSet(Word address)
{

	address &= 0x1FFE;
	int tile = address >> 4;
	Byte y = (address >> 1) & 0x7;

	for (int x = 0; x < 8; x++) {

		Byte xx = 0x80 >> x;

		Byte bit1 = (vram[address    ] & xx) ? 1 : 0;
		Byte bit2 = (vram[address + 1] & xx) ? 2 : 0;


		tileset[tile][y][x] = bit1 + bit2;
	}

}
