#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include "globals.h"

uintptr_t ammoDecAddress = 0;

void FindInfAmmoAddress() {
	std::cout << "searching for ammo dec address..." << std::endl;
	BYTE ammoDecBytes[] = { 0x89, 0x46, 0x3C, 0x48, 0x8B, 0x75, 0xF8 };
	int size;
	uintptr_t* addresses = AOB(ammoDecBytes, sizeof(ammoDecBytes), size);
	if (size > 0) {
		ammoDecAddress = addresses[0];
		std::cout << "ammo dec address found: " << std::hex << ammoDecAddress << std::dec << std::endl;
	}
	else {
		std::cout << "ammo dec address not found" << std::endl;
	}
}

void InfAmmo() {
	if (!ammoDecAddress)
	{
		return;
	}

	if (config.infAmmo)
	{
		std::cout << "enabled" << std::endl;
		byte ammoDecBytes[] = {0x90, 0x90, 0x90};
		WBM(ammoDecAddress, ammoDecBytes, 3);
	} else
	{
		std::cout << "disabled" << std::endl;
		byte ammoDecBytes[] = { 0x89, 0x46, 0x3C };
		WBM(ammoDecAddress, ammoDecBytes, 3);
	}
}