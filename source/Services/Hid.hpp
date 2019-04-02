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
#include <switch.h>
#include "Power.hpp"
#include "App.hpp"

#define LONGPRESS_MS 1500
#define SHORTPRESS_MS 200

class Hid
{
    public:
        static bool IsTouched();
		static bool IsTouched(SDL_Rect pos);
        static touchPosition GetTouchPos();
		static bool IsLongPress();
		static void InitGpioButton(GpioPadSession *but, GpioPadName name);
		static void CloseGpioButton(GpioPadSession *but);
};