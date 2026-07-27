#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"
#include "hacks.h"

void Damage() {

	static bool lastState = false;

	if (config.Damage)
		for (int i = 0;i < 3;i++)
		{
			uintptr_t weapon = RPM<uintptr_t>(A::shootctr + 0x40 + i * 0x8);
			WPM<int>(weapon + 0x1C, 1000);
		}
	else if (lastState)
		for (int i = 0;i < 3;i++)
		{
			uintptr_t weapon = RPM<uintptr_t>(A::shootctr + 0x40 + i * 0x8);
			WPM<int>(weapon + 0x18, 4);
		}

	lastState = config.Damage;
}
