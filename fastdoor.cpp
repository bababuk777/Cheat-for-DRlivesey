#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"

void FastDoor() {
    A::GameManager = RPM<uintptr_t>(A::InventoryListAdress + 0x40);
    WPM<float>(A::GameManager + 0x240, 5.0f);
}