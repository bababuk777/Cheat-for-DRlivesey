#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"
#include "hacks.h"


uintptr_t DamageAdress = 0;

void Finddamageadress() {
	std::vector<DWORD> offsets = { 0x48,0x40 };
	DamageAdress = ReadOffsetValue<uintptr_t>(InventoryListAdress, offsets)+0x18;
}

void Damage() {
	if (InventoryListAdress == 0 || RPM<uintptr_t>(InventoryListAdress) == 0) {
		
	}
	if (config.Damage)
		WPM<int>(DamageAdress, 1000);
	else
		WPM<int>(DamageAdress, 4);
}
