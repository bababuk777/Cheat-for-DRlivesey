#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"


uintptr_t HealthAdress = 0; 

void Findhealthadress() {
	std::vector<DWORD> offsets = { 0x40,0x20 };
	HealthAdress = ReadOffsetValue<uintptr_t>(A::InventoryListAdress, offsets);
}

void InfinityHP() {

	uintptr_t HPAddress = HealthAdress + 0x18;
	uintptr_t MaxHPAddress = HealthAdress + 0x1C;
	uintptr_t GoldHPAddress = HealthAdress + 0x68;
	uintptr_t GoldHPMaxAddress = HealthAdress + 0x6C;

	static bool lastState = false;
	if (config.InfinityHP) {
		WPM<int>(HPAddress, 1000);
		WPM<int>(MaxHPAddress, 1000);
		WPM<int>(GoldHPAddress, 1000);
		WPM<int>(GoldHPMaxAddress, 1000);
	}
	else if(lastState) {
		WPM<int>(HPAddress, 20);
		WPM<int>(MaxHPAddress, 20);
		WPM<int>(GoldHPAddress, 0);
		WPM<int>(GoldHPMaxAddress, 20);
	}
	lastState = config.InfinityHP;
}



void Invisible() {

	uintptr_t adress = HealthAdress + 0x64;

	static bool lastState = false;

	if (config.Invisible) {
		WPM<byte>(adress, 1);
	}
	else if(lastState) {
		WPM<byte>(adress, 0);
	}
	lastState = config.Invisible;
}