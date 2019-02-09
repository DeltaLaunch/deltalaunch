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
#include <SDL2/SDL_ttf.h>
#include <string>
#include <switch.h>
#include "UI/Draw.hpp"
#include "UI/Renderer.hpp"

class Debug
{
    public:
        Debug(TTF_Font *font, bool debugMode);
        ~Debug();
        void Print(Renderer rend, std::string text);
		void Clear();
        void Toggle();
	private:
		TTF_Font *Font;
		unsigned X, Y;
        bool debugOn;
};