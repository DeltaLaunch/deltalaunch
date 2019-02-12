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