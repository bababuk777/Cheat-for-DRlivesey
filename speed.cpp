#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"

void Speedhack() {
	uintptr_t speedadress = RPM<uintptr_t>(InventoryListAdress + 0x50);
	WPM<float>(speedadress + 0x64, config.Speedhack);
	WPM<float>(speedadress + 0x5C, config.Speedhack);
	
}