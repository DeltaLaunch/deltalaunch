/*
* ΔLaunch
* Copyright (C) 2018  Reisyukaku
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Power.hpp"

Result Power::Shutdown() {
    #ifdef __SWITCH__
    appletSetHandlesRequestToDisplay(false);
	appletStartShutdown();    
    #endif
    return 0;
}

Result Power::Reboot() {
    #ifdef __SWITCH__
    appletSetHandlesRequestToDisplay(false);
	appletStartReboot();
    #endif
    return 0;
}

Result Power::Sleep() {
    #ifdef __SWITCH__
    appletStartSleepSequence();
    #endif
    return 0;
}

u32 Power::GetBatteryLife() {
	u32 pwr = 100;
    #ifdef __SWITCH__
    psmInitialize();
	psmGetBatteryChargePercentage(&pwr);
    psmExit();
    #endif
	return pwr;
}