#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"


uintptr_t musicAddress = 0;

void Findmusicadress() {
	std::vector<DWORD> offsets = { 0x40,0xD8 };
	musicAddress = ReadOffsetValue<uintptr_t>(A::InventoryListAdress, offsets);
}

void Music()
{
    static bool lastState = false;

    uintptr_t end = musicAddress + 0x40;
    uintptr_t start = musicAddress + 0x3C;

    if (config.Music)
    {
        WPM<float>(end, 0.0f);
        WPM<float>(start, 0.0f);
    }
    else if (lastState)
    {
        WPM<float>(end, 30.0f);
        WPM<float>(start, 60.0f);
    }

    lastState = config.Music;
}