#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"

uintptr_t Bossadress = 0;

void FindFBossadress() {
	uintptr_t Baseadress = GetModuleBaseAddress(L"GameAssembly.dll");
	std::vector<DWORD> offsets = { 0x01681C00, 0xB8, 0x0, 0x40, 0x18 };
	Bossadress = ReadOffsetValue<uintptr_t>(Baseadress, offsets) + 0x18;
}

void Boss() {
	if (config.Boss)
		WPM<int>(Bossadress, 0 );
	else
		WPM<int>(Bossadress, 500);
}
