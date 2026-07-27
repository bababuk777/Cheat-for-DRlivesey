#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"


void GhostMod() {

	static bool lastState = false;

	if (config.GhostMod)
		WPM<float>(A::Freewalladress, 4.0f);

	else if (lastState)
		WPM<float>(A::Freewalladress, 1.95f);

	lastState = config.GhostMod;
}
