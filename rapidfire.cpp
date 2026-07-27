#include "config.h"
#include <iostream>
#include <Windows.h>
#include "memory_functions.h"
#include <vector>
#include "globals.h"


void RapidFire()
{
    static bool lastState = false;

    if (config.RapidFire){

        for (size_t i = 0;i < 3;i++)
        {
            uintptr_t weapon = RPM<uintptr_t>(A::shootctr + 0x40 + i * 0x8);
            WPM<float>(weapon + 0x1C, 0.0f);
            WPM<float>(weapon + 0x20, 0.0f);
        }
    }
    else if (lastState){

        for (size_t i = 0;i < 3;i++)
        {
            uintptr_t weapon = RPM<uintptr_t>(A::shootctr + 0x40 + i * 0x8);
            if (i==2){
                WPM<float>(weapon + 0x1C, 1.25f);
                WPM<float>(weapon + 0x20, 1.25f);
                break;
            }
            WPM<float>(weapon + 0x1C, 0.2f);
            WPM<float>(weapon + 0x20, 0.2f);
        }
    }

    lastState = config.RapidFire;
}