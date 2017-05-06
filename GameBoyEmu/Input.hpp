#pragma once
#include <SDL.h>

#include "Register.hpp"

class Input {
	
	int keys1[4] = {
		SDL_SCANCODE_TAB,
		SDL_SCANCODE_SPACE,
		SDL_SCANCODE_X,
		SDL_SCANCODE_Z
	};

	int keys2[4] = {
		SDL_SCANCODE_DOWN,
		SDL_SCANCODE_UP,
		SDL_SCANCODE_LEFT,
		SDL_SCANCODE_RIGHT
	};


	Byte keyRow1;
	Byte keyRow2;
	Byte keyCode;
	bool debugShow = false;
	bool debugSave = false;

public:

	void updateKeys() {
		const Uint8* stats = SDL_GetKeyboardState(NULL);
		keyRow1 = 0xFF;
		keyRow2 = 0xFF;

		for (int i = 0; i < 4; i++) {
			if (stats[keys1[i]]) {
				keyRow1 ^= (8 >> i);
			}
		}

		for (int i = 0; i < 4; i++) {
			if (stats[keys2[i]]) {
				keyRow2 ^= (8 >> i);
			}
		}

		if (stats[SDL_SCANCODE_0]) {
			debugShow = true;
		}

		if (stats[SDL_SCANCODE_1]) {
			debugSave = true;
		}

	}
	Byte getKeys() {
		

		switch (keyCode & 0xF0) {
		case 0x10:
			return (0x10 ^ keyRow1);
			break;
		case 0x20:
			return (0x20 ^ keyRow2);
			break;
		default:
			__debugbreak();
			break;
		}
	}

	void setKeyCode(Byte keyCode) {
		this->keyCode = keyCode;
	}

	bool debugEnable() {
		return debugShow;
	}

	void setdebugEnable() {
		debugShow = true;
	}

	bool debugSaveing() {
		return debugSave;
	}

};