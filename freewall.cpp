#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"

uintptr_t Freewalladress = 0;

void FindFreewalladress() {
	uintptr_t Baseadress = GetModuleBaseAddress(L"UnityPlayer.dll");
	std::vector<DWORD> offsets = { 0x01B41E40, 0x48, 0x0, 0x60, 0x0};
	Freewalladress = ReadOffsetValue<uintptr_t>(Baseadress, offsets)+ 0x1FC;
}


void Freewall() {
	if (config.Freewall)
		WPM<float>(Freewalladress, 4.0f);
	else 
		WPM<float>(Freewalladress, 2.0f);
}
