#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include "globals.h"

uintptr_t invisiblienemyDecAddress = 0;

void FindinvisiblienemyAddress() {
	BYTE opcodses[] = { 0x01, 0x43, 0x18, 0x83, 0x7B, 0x18, 0x00, 0x7D, 0x07, 0xC7, 0x43, 0x18, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x43, 0x70 };
	int size;
	uintptr_t* addresses = AOB(opcodses, sizeof(opcodses), size);
	invisiblienemyDecAddress = addresses[0];
}


void Invisiblienemy() {

	if (config.Invisiblienemy)
	{
		std::cout << "enabled" << std::endl;
		byte invisiblienemyDecBytes[] = { 0x90, 0x90, 0x90 };
		WBM(invisiblienemyDecAddress, invisiblienemyDecBytes, 3);
	}
	else
	{
		std::cout << "disabled" << std::endl;
		byte invisiblienemyDecBytes[] = { 0x01, 0x43, 0x18 };
		WBM(invisiblienemyDecAddress, invisiblienemyDecBytes, 3);
	}
}