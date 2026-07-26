#pragma once


#include <windows.h>
#include <vector>
#include <string>

extern HANDLE hProcess;

bool AttachToProcess();

template<typename T> T RPM(SIZE_T address) {
	T buffer;
	ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(T), NULL);
	return buffer;
}

template<typename T> void WPM(SIZE_T address, T value) {
	WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(T), NULL);
}

template<typename T>
T ReadOffsetValue(uintptr_t baseAddress, std::vector<DWORD> offsets) {
	uintptr_t currentPtr = baseAddress + offsets[0];
	currentPtr = RPM<uintptr_t>(currentPtr);
	for (size_t i = 1; i < offsets.size() - 1; i++) {
		currentPtr += offsets[i];
		currentPtr = RPM<uintptr_t>(currentPtr);
	}
	currentPtr += offsets.back();
	return RPM<T>(currentPtr);
}


byte* RBM(SIZE_T address, size_t size);

BOOL WBM(SIZE_T address, const byte* bytes, size_t size);
std::string ReadMemoryString(SIZE_T address, SIZE_T size);

uintptr_t GetModuleBaseAddress(const wchar_t* modName);

typedef struct _FOUND_ADDRESSES {
	UINT_PTR* Addresses;
	UINT_PTR    NumberOfAddresses;
} FOUND_ADDRESSES, * PFOUND_ADDRESSES;

BOOLEAN
FindAddressOfByteArray(
	PFOUND_ADDRESSES    FoundAddresses,
	BYTE* Data,
	INT                 DataSize,
	BOOLEAN             SkipCompleteMatches
);

typedef struct _MODULE_RANGE {
	UINT_PTR    FirstAddress;
	UINT_PTR    LastAddress;
} MODULE_RANGE, * PMODULE_RANGE;

BOOLEAN
FindProcessIdByName(
	LPCWSTR     ProcessName,
	PDWORD      ProcessId
);

BOOLEAN
FindAddressOfByteArrayInProcessModule(
	PFOUND_ADDRESSES    FoundAddresses,
	HANDLE              ProcessHandle,
	BYTE* Data,
	INT                 DataSize,
	BOOLEAN             SkipCompleteMatches,
	LPCWSTR             ModuleName
);

BOOLEAN FindAddressOfByteArrayInEntireProcess(
	PFOUND_ADDRESSES FoundAddresses,
	BYTE* Data,
	INT DataSize,
	BOOLEAN SkipCompleteMatches = true
);

//uintptr_t

uintptr_t* AOB(byte* bytes, size_t bytesSize, int& outSize);