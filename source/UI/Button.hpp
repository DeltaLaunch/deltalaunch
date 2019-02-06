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
#include <functional>
#include <string>
#include <switch.h>

class Button
{
    public:
        Button(std::string name, uint32_t x, uint32_t y, std::function<void()> callback);
        Button(uint32_t x, uint32_t y, uint32_t w, uint32_t h, std::function<void()> callback);
		~Button();
		void Run();
        void SetText(std::string txt);
        void SetColor(SDL_Color col);
		
		//vars
        uint32_t X;
		uint32_t Y;
        uint32_t H;
		uint32_t W;
        std::string Sprite;
	private:
        std::string Text;
        SDL_Color Color;
		std::function<void()> Callback;
};