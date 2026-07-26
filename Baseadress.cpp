#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"

void FindInventoryList() {
	uintptr_t Baseadress = GetModuleBaseAddress(L"UnityPlayer.dll");
	std::vector<DWORD> offsets = { 0x01B04470, 0x120, 0x38, 0x60, 0x38};
	InventoryListAdress = ReadOffsetValue<uintptr_t>(Baseadress, offsets);
}

