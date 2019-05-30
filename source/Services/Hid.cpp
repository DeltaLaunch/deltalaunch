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
TouchInfo Hid::touchInfo;

void Hid::KeyProcess() {
    hidScanInput();
    Input = hidKeysDown(CONTROLLER_P1_AUTO);
}

void Hid::TouchInit() {
    touchInfo.state = TouchInfo::TouchNone;
    touchInfo.tapType = TouchInfo::TapNone;
}

void Hid::TouchProcess() {
    touchPosition currentTouch;
    u32 touches = hidTouchCount();
    
    if (touches >= 1)
        hidTouchRead(&currentTouch, 0);
        
    u64 current_time;
    timeGetCurrentTime(TimeType_UserSystemClock, &current_time);
    
    // On touch start.
    if (touches == 1 && (touchInfo.state == TouchInfo::TouchNone || touchInfo.state == TouchInfo::TouchEnded)) {
        touchInfo.state = TouchInfo::TouchStart;
        touchInfo.firstTouch = currentTouch;
        touchInfo.prevTouch = currentTouch;
        touchInfo.tapType = TouchInfo::TapShort;
        touchInfo.touchStart = current_time;
    }
    // On touch moving.
    else if (touches >= 1 && touchInfo.state != TouchInfo::TouchNone) {
        touchInfo.state = TouchInfo::TouchMoving;
        touchInfo.prevTouch = currentTouch;

        if (touchInfo.tapType != TouchInfo::TapNone && (abs((double)(touchInfo.firstTouch.px - currentTouch.px)) > TAP_MOVEMENT_GAP || abs((double)(touchInfo.firstTouch.py - currentTouch.py)) > TAP_MOVEMENT_GAP))
            touchInfo.tapType = TouchInfo::TapNone;
        else if (touchInfo.tapType == TouchInfo::TapShort && current_time - touchInfo.touchStart >= LONG_TAP_PERIOD)
            touchInfo.tapType = TouchInfo::TapLong;
    }
    // On touch end.
    else
        touchInfo.state = (touchInfo.state == TouchInfo::TouchMoving) ? TouchInfo::TouchEnded : TouchInfo::TouchNone;
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