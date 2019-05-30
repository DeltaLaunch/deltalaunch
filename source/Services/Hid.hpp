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

#pragma once
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdlib.h>
#include <switch.h>
#include "Power.hpp"
#include "App.hpp"

#define TAP_MOVEMENT_GAP 20
#define LONG_TAP_PERIOD 1.5

#define tapped_inside(touchInfo, x1, y1, x2, y2) (touchInfo.firstTouch.px >= x1 && touchInfo.firstTouch.px <= x2 && touchInfo.firstTouch.py >= y1 && touchInfo.firstTouch.py <= y2)
#define tapped_outside(touchInfo, x1, y1, x2, y2) (touchInfo.firstTouch.px < x1 || touchInfo.firstTouch.px > x2 || touchInfo.firstTouch.py < y1 || touchInfo.firstTouch.py > y2)

class TouchInfo {
    public:
        typedef enum TouchState 
        {
            TouchNone,
            TouchStart,
            TouchMoving,
            TouchEnded
        } TouchState;

        typedef enum TapType 
        {
            TapNone,
            TapShort,
            TapLong
        } TapType;
    
        TouchState state;
        touchPosition firstTouch;
        touchPosition prevTouch;
        TapType tapType;
        u64 touchStart;
};

class Hid {
    public:
        static u64 Input;
        static void KeyProcess();
        static void TouchInit();
        static void TouchProcess();
        static touchPosition GetTouchPos() { return touchInfo.prevTouch; };
        static bool IsTouched(SDL_Rect pos) { return (tapped_inside(touchInfo, (unsigned)pos.x, (unsigned)pos.y, (unsigned)(pos.x+pos.w), (unsigned)(pos.y+pos.h)) && touchInfo.state == TouchInfo::TouchStart); };
        static bool IsTouched() { return (touchInfo.tapType == TouchInfo::TapShort && touchInfo.state == TouchInfo::TouchStart);  };
		static bool IsLongPress() { return (touchInfo.tapType == TouchInfo::TapLong && touchInfo.state == TouchInfo::TouchStart); };
        static bool IsMoving() { return touchInfo.state == TouchInfo::TouchMoving; }
        static bool IsMoving(SDL_Rect pos) { return (tapped_inside(touchInfo, (unsigned)pos.x, (unsigned)pos.y, (unsigned)(pos.x+pos.w), (unsigned)(pos.y+pos.h)) && touchInfo.state == TouchInfo::TouchMoving); }
        static bool IsGpioPressed(GpioPadSession *but, GpioPadName name);
		static void InitGpioButton(GpioPadSession *but, GpioPadName name);
		static void CloseGpioButton(GpioPadSession *but);
    private:
        static TouchInfo touchInfo;
};