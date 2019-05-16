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

#include "Hid.hpp"

u64 Hid::Input;

u64 Hid::GetInput() {
    hidScanInput();
    return hidKeysDown(CONTROLLER_P1_AUTO);
}

bool Hid::IsTouched(){
    if(hidTouchCount() < 1) return false;
    touchPosition touchPos;
    hidTouchRead(&touchPos, 0);
    return (touchPos.px != 0 && touchPos.py != 0);
}

bool Hid::IsTouched(SDL_Rect pos){
    if(hidTouchCount() < 1) return false;
    touchPosition touchPos;
    hidTouchRead(&touchPos, 0);
    return ((int)touchPos.px >= pos.x && (int)touchPos.px <= (pos.x + pos.w) && (int)touchPos.py >= pos.y && (int)touchPos.py <= (pos.y + pos.h));
}

touchPosition Hid::GetTouchPos() {
    touchPosition touchPos;
    hidTouchRead(&touchPos, 0);
    return touchPos;
}

bool Hid::IsLongPress() {
	touchPosition touchPos;
    hidTouchRead(&touchPos, 0);
	int i = svcGetSystemTick();
    while(touchPos.px != 0 && touchPos.py != 0) {
		if(svcGetSystemTick() - i >= LONGPRESS_MS) return true;
		hidTouchRead(&touchPos, 0);
	}
	return false;
}

bool Hid::IsGpioPressed(GpioPadSession *but, GpioPadName name) {
    GpioValue gval = GpioValue_High;
    gpioPadGetValue(but, &gval);
    return (gval == GpioValue_Low);
}

void Hid::InitGpioButton(GpioPadSession *but, GpioPadName name) {
	gpioOpenSession(but, name);
}

void Hid::CloseGpioButton(GpioPadSession *but) {
	gpioPadClose(but);
}