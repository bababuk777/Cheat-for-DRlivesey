#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"


uintptr_t musicAdress = 0;

void Findmusicadress() {
	std::vector<DWORD> offsets = { 0x40,0xD8 };
	musicAdress = ReadOffsetValue<uintptr_t>(InventoryListAdress, offsets);
}

void Music() {
	uintptr_t end = musicAdress + 0x40;
	uintptr_t start = musicAdress + 0x3C;
	if (config.Music) {
		WPM<float>(end, 0.0f);
		WPM<float>(start, 0.0f);
	}
	else {
		WPM<float>(end, 30.0f);
		WPM<float>(start, 60.0f);
	}
}