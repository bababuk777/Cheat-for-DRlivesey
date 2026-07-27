#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"

void Speedhack()
{
    uintptr_t plrCtrl = RPM<uintptr_t>(A::InventoryListAdress + 0x50);

    WPM<float>(plrCtrl + 0x5C, config.Speedhack);
    WPM<float>(plrCtrl + 0x64, config.Speedhack);
}