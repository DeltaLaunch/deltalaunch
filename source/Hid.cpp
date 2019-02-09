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

void Hid::Check() {
    hidScanInput();
        
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

    if (kDown & KEY_PLUS) {
        Power::Shutdown();
    }
    
    if (kDown & KEY_MINUS) {
        App::LaunchApplet(AppletId_shop, LibAppletMode_AllForeground);
    }
    
    if ((kDown & KEY_DUP) || (kDown & KEY_LSTICK_UP)) {
        
    }
    if ((kDown & KEY_DDOWN) || (kDown & KEY_LSTICK_DOWN)) {
        
    }
}

bool Hid::IsTouched(u32 x1, u32 y1, u32 x2, u32 y2){
    touchPosition touchPos;
    hidTouchRead(&touchPos, 0);
    return (touchPos.px >= x1 && touchPos.px <= x2 && touchPos.py >= y1 && touchPos.py <= y2);
}

touchPosition Hid::GetTouchPos() {
    touchPosition touchPos;
    hidTouchRead(&touchPos, 0);
    return touchPos;
}