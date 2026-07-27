#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"


void Kill_All() {
    std::vector<DWORD> offsets = { 0x01681C00, 0xB8, 0x0, 0x40, 0x18 };

    uintptr_t Bossadress = ReadOffsetValue<uintptr_t>(A::GameAssemblyBaseadress, offsets) + 0x18;
    uintptr_t SpawnedEnemes = RPM<uintptr_t>(A::CurRoom + 0x30);
    uintptr_t SpawnedEnemesAdd = RPM<uintptr_t>(A::CurRoom + 0x38);

    WPM<int>(SpawnedEnemes + 0x18, 0);
    WPM<int>(SpawnedEnemesAdd + 0x18, 0);
    WPM<int>(Bossadress, 0);
}