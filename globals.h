#pragma once
#include <cstdint>
#include "math.h"

namespace A {
    inline uintptr_t UnityPlayerBaseadress = 0;
    inline uintptr_t GameAssemblyBaseadress = 0;
    inline uintptr_t InventoryListAdress = 0;
    inline uintptr_t cosyewAddress = 0, sinyewAddress = 0;
    inline float rotation = 0;
    inline Vector3 PlayerPosition = {};
    inline uintptr_t EntPositionlist = 0;
    inline intptr_t shootctr = 0;
    inline uintptr_t GameManager = 0;
    inline uintptr_t CurRoom = 0;
    inline uintptr_t Freewalladress = 0;
}

void FindBaseAddres();

void UpdateinStartRoom();

void FindInventoryList();
void Findmusicadress();
void Findhealthadress();
void FindShootctrAddress();
void FindEntPositionlist();
void FindCurRoom();
void UpdateEnemies();

void FindRotationAddress();
void FindPlayerPosition();

