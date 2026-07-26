#include "memory_functions.h"

#include <TlHelp32.h>
#include <windows.h>
#include <vector>
#include <string>
#include <Windows.h>

#include "offsets.h"

HANDLE hProcess = nullptr;
DWORD processId = -1;

DWORD GetProcId(const wchar_t* procName){
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));

		}
	}
	CloseHandle(hSnap);
	return procId;
}

bool AttachToProcess() {
	processId = GetProcId(EXE_NAME);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	return hProcess != nullptr;
}

byte* RBM(SIZE_T address, size_t size) {
	byte* buffer = new byte[size];
	ReadProcessMemory(hProcess, (LPCVOID)address, buffer, size, NULL);
	return buffer;
}

BOOL WBM(SIZE_T address, const byte* bytes, size_t size) {
	return WriteProcessMemory(hProcess, (LPVOID)address, bytes, size, NULL);
}

std::string ReadMemoryString(SIZE_T address, SIZE_T size) {
	char* buffer = new char[size + 1];
	SIZE_T bytesRead;

	if (ReadProcessMemory(hProcess, (LPVOID)address, buffer, size, &bytesRead) && bytesRead > 0) {
		buffer[bytesRead] = '\0';
		std::string result(buffer);
		delete[] buffer;
		return result;
	}
	else {
		delete[] buffer;
		return "";
	}
}

uintptr_t GetModuleBaseAddress(const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}




BOOLEAN
FindAddressOfByteArray(
	PFOUND_ADDRESSES    FoundAddresses,
	BYTE* Data,
	INT                 DataSize,
	BOOLEAN             SkipCompleteMatches
)
{
	if ((NULL == hProcess) || (NULL == Data) || (NULL == DataSize)) {
		return FALSE;
	}

	DWORD dwReadableMask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY
		| PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE
		| PAGE_EXECUTE_WRITECOPY);
	DWORD dwProtectedMask = (PAGE_GUARD | PAGE_NOACCESS);

	INT                         iFoundSize = 10;
	UINT_PTR                    ulCurrAddr = NULL;
	BYTE* lpBuff = NULL;
	SIZE_T                      ulBytesRead = NULL;

	SYSTEM_INFO                 SysInfo;
	MEMORY_BASIC_INFORMATION    Mbi;

	ZeroMemory(&SysInfo, sizeof(SysInfo));
	ZeroMemory(&Mbi, sizeof(Mbi));

	FoundAddresses->Addresses = (UINT_PTR*)(malloc(iFoundSize * sizeof(UINT_PTR)));

	GetSystemInfo(&SysInfo);

	ulCurrAddr = (UINT_PTR)(SysInfo.lpMinimumApplicationAddress);

	while ((sizeof(Mbi) == VirtualQueryEx(hProcess, (LPVOID)(ulCurrAddr), &Mbi, sizeof(Mbi)),
		(ulCurrAddr <= (UINT_PTR)(SysInfo.lpMaximumApplicationAddress)))) {

		if ((dwReadableMask & Mbi.Protect) && (FALSE == (dwProtectedMask & Mbi.Protect))) {
			lpBuff = (BYTE*)(malloc(Mbi.RegionSize));

			if (TRUE == ReadProcessMemory(hProcess, (LPVOID)(ulCurrAddr), lpBuff, Mbi.RegionSize,
				&ulBytesRead)) {

				if (ulBytesRead == Mbi.RegionSize) {

					for (UINT i = 0; i < Mbi.RegionSize; ++i) {

						if (0 == memcmp((LPCVOID)(lpBuff + i), Data, DataSize)) {
							if (iFoundSize == (FoundAddresses->NumberOfAddresses + 1)) {
								LPVOID lpTemp = realloc(FoundAddresses->Addresses, (iFoundSize += 50)
									* sizeof(UINT_PTR));

								if (NULL == lpTemp) {
									free(FoundAddresses->Addresses);
									free(lpBuff);

									return FALSE;
								}

								FoundAddresses->Addresses = (UINT_PTR*)(lpTemp);
							}

							FoundAddresses->Addresses[++FoundAddresses->NumberOfAddresses]
								= (ulCurrAddr + i);

							if (TRUE == SkipCompleteMatches) {
								i += DataSize;
							}
						}
					}
				}
			}

			free(lpBuff);
		}

		ulCurrAddr = (UINT_PTR)(Mbi.BaseAddress) + Mbi.RegionSize;
	}
	return TRUE;
}

BOOLEAN
FindAddressOfByteArrayWithDelimiters(
	PFOUND_ADDRESSES    FoundAddresses,
	HANDLE              ProcessHandle,
	BYTE* Data,
	INT                 DataSize,
	BOOLEAN             SkipCompleteMatches,
	UINT_PTR            FirstAddress,
	UINT_PTR            LastAddress
)
{
	if ((NULL == ProcessHandle) || (NULL == Data) || (NULL == DataSize)) {
		return FALSE;
	}

	DWORD dwReadableMask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY
		| PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE
		| PAGE_EXECUTE_WRITECOPY);
	DWORD dwProtectedMask = (PAGE_GUARD | PAGE_NOACCESS);

	INT                         iFoundSize = 10;
	UINT_PTR                    ulCurrAddr = NULL;
	BYTE* lpBuff = NULL;
	SIZE_T                      ulBytesRead = NULL;

	MEMORY_BASIC_INFORMATION    Mbi;
	ZeroMemory(&Mbi, sizeof(Mbi));

	FoundAddresses->Addresses = (UINT_PTR*)(malloc(iFoundSize * sizeof(UINT_PTR)));

	ulCurrAddr = (UINT_PTR)(FirstAddress);

	while ((sizeof(Mbi) == VirtualQueryEx(ProcessHandle, (LPVOID)(ulCurrAddr), &Mbi, sizeof(Mbi))),
		(ulCurrAddr <= LastAddress)) {
		if ((dwReadableMask & Mbi.Protect) && (FALSE == (dwProtectedMask & Mbi.Protect))) {
			lpBuff = (BYTE*)(malloc(Mbi.RegionSize));

			if (TRUE == ReadProcessMemory(ProcessHandle, (LPVOID)(ulCurrAddr), lpBuff, Mbi.RegionSize,
				&ulBytesRead)) {

				if (ulBytesRead == Mbi.RegionSize) {

					for (UINT i = 0; i < Mbi.RegionSize; ++i) {

						if (0 == memcmp((LPCVOID)(lpBuff + i), Data, DataSize)) {
							if (iFoundSize == (FoundAddresses->NumberOfAddresses + 1)) {
								LPVOID lpTemp = realloc(FoundAddresses->Addresses, (iFoundSize += 50)
									* sizeof(UINT_PTR));

								if (NULL == lpTemp) {
									free(FoundAddresses->Addresses);
									free(lpBuff);

									return FALSE;
								}

								FoundAddresses->Addresses = (UINT_PTR*)(lpTemp);
							}

							FoundAddresses->Addresses[FoundAddresses->NumberOfAddresses]
								= (ulCurrAddr + i);

							++FoundAddresses->NumberOfAddresses;

							if (TRUE == SkipCompleteMatches) {
								i += DataSize;
							}
						}
					}
				}
			}

			free(lpBuff);
		}

		ulCurrAddr = (UINT_PTR)(Mbi.BaseAddress) + Mbi.RegionSize;
	}

	return TRUE;
}

BOOLEAN FindAddressOfByteArrayInEntireProcess(
	PFOUND_ADDRESSES FoundAddresses,
	BYTE* Data,
	INT DataSize,
	BOOLEAN SkipCompleteMatches
) {
	SYSTEM_INFO sysInfo;
	ZeroMemory(&sysInfo, sizeof(sysInfo));

	GetSystemInfo(&sysInfo);

	return FindAddressOfByteArrayWithDelimiters(FoundAddresses, hProcess, Data, DataSize, SkipCompleteMatches,
		(UINT_PTR)sysInfo.lpMinimumApplicationAddress, (UINT_PTR)sysInfo.lpMaximumApplicationAddress);
}

BOOLEAN
FindModuleRangeInProcess(
	DWORD           ProcessId,
	LPCWSTR         ModuleName,
	PMODULE_RANGE   Range
) {
	if ((NULL == ModuleName) || (L'\0' == *ModuleName) || (NULL == ProcessId)) {
		return FALSE;
	}

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessId);

	if (NULL == hSnap) {
		return FALSE;
	}

	BOOLEAN             bFoundModule = FALSE;
	BOOLEAN             bFoundNextModule = FALSE;

	MODULEENTRY32W      ME32;
	ZeroMemory(&ME32, sizeof(ME32));
	ME32.dwSize = sizeof(ME32);

	bFoundNextModule = Module32FirstW(hSnap, &ME32);
	while (TRUE == bFoundNextModule) {

		if (NULL == _wcsicmp(ModuleName, ME32.szModule)) {
			Range->FirstAddress = (UINT_PTR)ME32.modBaseAddr;
			Range->LastAddress = (UINT_PTR)ME32.modBaseAddr + ME32.modBaseSize;
			bFoundModule = TRUE;
			break;
		}

		bFoundNextModule = Module32NextW(hSnap, &ME32);
	}

	CloseHandle(hSnap);

	if (TRUE == bFoundModule) {
		return TRUE;
	}

	return FALSE;
}

BOOLEAN
FindAddressOfByteArrayInProcessModule(
	PFOUND_ADDRESSES    FoundAddresses,
	HANDLE              ProcessHandle,
	BYTE* Data,
	INT                 DataSize,
	BOOLEAN             SkipCompleteMatches,
	LPCWSTR             ModuleName
) {
	MODULE_RANGE modRange;
	ZeroMemory(&modRange, sizeof(modRange));

	DWORD dwProcId = GetProcessId(ProcessHandle);

	if (NULL == dwProcId) {
		return FALSE;
	}

	if (FALSE == FindModuleRangeInProcess(dwProcId, ModuleName, &modRange)) {
		return FALSE;
	}

	return FindAddressOfByteArrayWithDelimiters(FoundAddresses, ProcessHandle, Data, DataSize, SkipCompleteMatches, modRange.FirstAddress,
		modRange.LastAddress);
}


BOOLEAN
FindProcessIdByName(
	LPCWSTR     ProcessName,
	PDWORD      ProcessId
) {
	if ((NULL == ProcessName) || (L'\0' == *ProcessName) || (NULL == ProcessId)) {
		return FALSE;
	}

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (NULL == hSnap) {
		return FALSE;
	}

	BOOLEAN             bFoundProcess = FALSE;
	BOOLEAN             bFoundNextProcess = FALSE;

	PROCESSENTRY32W     PE32;
	ZeroMemory(&PE32, sizeof(PE32));
	PE32.dwSize = sizeof(PE32);

	bFoundNextProcess = Process32FirstW(hSnap, &PE32);
	while (TRUE == bFoundNextProcess) {

		if (NULL == _wcsicmp(ProcessName, PE32.szExeFile)) {
			*ProcessId = PE32.th32ProcessID;
			bFoundProcess = TRUE;
			break;
		}

		bFoundNextProcess = Process32NextW(hSnap, &PE32);
	}

	CloseHandle(hSnap);

	if (TRUE == bFoundProcess) {
		return TRUE;
	}

	return FALSE;
}

uintptr_t* AOB(byte* bytes, size_t bytesSize, int& outSize) {
	FOUND_ADDRESSES faFound;
	ZeroMemory(&faFound, sizeof(faFound));
	if (false == FindAddressOfByteArrayInEntireProcess(&faFound, bytes, bytesSize)) {
		outSize = 0;
		return nullptr;
	}

	outSize = faFound.NumberOfAddresses;

	uintptr_t* addresses = new uintptr_t[outSize];
	for (UINT i = 0; i < outSize; ++i) {
		addresses[i] = faFound.Addresses[i];
	}

	return addresses;
}
