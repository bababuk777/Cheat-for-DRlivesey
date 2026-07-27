#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"
#include "math.h"

#include "hacks.h"[a


void FindBaseAddres() {
	A::UnityPlayerBaseadress = GetModuleBaseAddress(L"UnityPlayer.dll");
	A::GameAssemblyBaseadress = GetModuleBaseAddress(L"GameAssembly.dll");
}
//
//bool CheckStartRoom()
//{
//	static bool wasInStartRoom = false;
//
//	std::vector<DWORD> offset1s = { 0x01B04470, 0x120, 0x38, 0x60, 0x38,  0x40, 0x248, 0x5B };
//	bool isStartRoom = ReadOffsetValue<bool>(A::UnityPlayerBaseadress, offset1s);
//
//	std::vector<DWORD> offsets = { 0x01B41E40, 0x48, 0x0, 0x60, 0x0 };
//	A::Freewalladress = ReadOffsetValue<uintptr_t>(A::UnityPlayerBaseadress, offsets) + 0x1FC;
//
//	if (A::Freewalladress != 0 && isStartRoom && !wasInStartRoom)
//	{
//		wasInStartRoom = true;
//		return true;       
//	}
//
//	if (!isStartRoom)
//		wasInStartRoom = false;
//
//	return false;
//}

void UpdateinStartRoom() {

	static bool previous = false;
	bool proverka = false;

	std::vector<DWORD> offset1s = { 0x01B04470, 0x120, 0x38, 0x60, 0x38,  0x40, 0x248, 0x5B };
	bool isStartRoom = ReadOffsetValue<bool>(A::UnityPlayerBaseadress, offset1s);

	std::vector<DWORD> offsets = { 0x01B41E40, 0x48, 0x0, 0x60, 0x0 };
	A::Freewalladress = ReadOffsetValue<uintptr_t>(A::UnityPlayerBaseadress, offsets) + 0x1FC;
	
	float check = RPM<float>(A::Freewalladress);
	if (check > 1.8 && check < 2.2)
		proverka = true;

	
	if (isStartRoom && !previous && proverka) {
		Sleep(100);
		FindBaseAddres();
		FindInventoryList();
		FindEntPositionlist();
		FindCurRoom();
		Findmusicadress();
		Findhealthadress();
		FindShootctrAddress();

		RapidFire();
		Music();
		InfinityHP();
		Invisible();
		GhostMod();
		Damage();
		Speedhack();
		if (config.AIM) AIM();
		if (config.KillAll) Kill_All();
		if (config.Radar) Radar();
		if (config.FastDoor) FastDoor();
		std::cout << "Start room entered — addresses updated\n";
	}

	previous = isStartRoom;
}

void FindInventoryList() {
	std::vector<DWORD> offsets = { 0x01B04470, 0x120, 0x38, 0x60, 0x38 };
	A::InventoryListAdress = ReadOffsetValue<uintptr_t>(A::UnityPlayerBaseadress, offsets)+ 0x0;
}

void FindShootctrAddress() {
	A::shootctr = RPM<uintptr_t>(A::InventoryListAdress + 0x48);
}

void FindRotationAddress() {
	std::vector<DWORD> offsets = { 0x01B41FF8, 0x430, 0x10, 0x28};
	uintptr_t Addres  = ReadOffsetValue<uintptr_t>(A::UnityPlayerBaseadress, offsets);
	A::cosyewAddress = Addres + 0x94;
	A::sinyewAddress = Addres + 0x9C;
	float cosyew = RPM<float>(A::cosyewAddress);
	float sinyew = RPM<float>(A::sinyewAddress);
	float angle = atan2f(sinyew, cosyew);
	if (angle < 0)
		angle += M_PI;
	A::rotation = angle*-2.0f;


}

void FindPlayerPosition() {
	std::vector<DWORD> offsets = { 0x01B41FF8, 0x430, 0x18, 0x28,0x150 };
	A::PlayerPosition = ReadOffsetValue<Vector3>(A::UnityPlayerBaseadress, offsets);
}

void FindEntPositionlist() {
	std::vector<DWORD> offsets = { 0x01B28D70, 0x90, 0xA0};
	A::EntPositionlist = ReadOffsetValue<uintptr_t>(A::UnityPlayerBaseadress, offsets);
}
void FindCurRoom(){
	std::vector<DWORD> offsets = { 0x40, 0x248 };
	A::CurRoom = ReadOffsetValue<uintptr_t>(A::InventoryListAdress, offsets);
}