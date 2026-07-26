#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"


uintptr_t HealthAdress = 0; 

void Findhealthadress() {
	std::vector<DWORD> offsets = { 0x40,0x20 };
	HealthAdress = ReadOffsetValue<uintptr_t>(InventoryListAdress, offsets);
}

void Basehealth() {
	uintptr_t adress = HealthAdress + 0x18;
	uintptr_t maxadress = HealthAdress + 0x1C;
	if (config.Basehealth) {
		WPM<int>(adress, 1000);
		WPM<int>(maxadress, 1000);
	}
	else {
		WPM<int>(adress, 20);
		WPM<int>(maxadress, 20);
	}
}

void Goldhealth() {
	uintptr_t adress = HealthAdress + 0x68;
	uintptr_t maxadress = HealthAdress + 0x6C;
	if (config.Goldhealth) {
		WPM<int>(adress, 1000);
		WPM<int>(maxadress, 1000);
	}
	else {
		WPM<int>(adress, 0);
		WPM<int>(maxadress, 0);
	}
}

void Invisible() {
	uintptr_t adress = HealthAdress + 0x64;
	if (config.Invisible) {
		WPM<byte>(adress, 1);
	}
	else {
		WPM<byte>(adress, 0);
	}
}