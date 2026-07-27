
#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include "globals.h"

uintptr_t noSpreadAddress = 0;
byte* sourceBytes = nullptr;

void FindNoSpreadAddress() {
	std::cout << "searching for no spread addresses..." << std::endl;
	BYTE noSpreadBytes[] = {0x41, 0xFF, 0xD3, 0x48, 0x8B, 0xD5, 0x48, 0x81, 0xC2, 0x30, 0xFF};
	{
		int size;
		uintptr_t* addresses = AOB(noSpreadBytes, sizeof(noSpreadBytes), size);
		if (size > 0) {
			noSpreadAddress = addresses[0] + 0x2E;
			std::cout << "found: " << std::hex << noSpreadAddress << std::dec << std::endl;
			sourceBytes = RBM(noSpreadAddress, 10);
		}
		else {
			std::cout << "not found" << std::endl;
		}
	}
}

void NoSpread() {
	if (!noSpreadAddress)
	{
		return;
	}

	if (config.noSpread)
	{
		std::cout << "enabled" << std::endl;
		byte noSpreadBytes[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
		WBM(noSpreadAddress, noSpreadBytes, 10);
	}
	else
	{
		std::cout << "disabled" << std::endl;
		WBM(noSpreadAddress, sourceBytes, 10);
	}
}