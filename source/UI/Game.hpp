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
#include <SDL2/SDL_image.h>
#include <functional>
#include <string>
#include <ios>
#include <sstream>
#include <switch.h>
#include "../Services/App.hpp"
#include "../Services/Account.hpp"

#define AQUA 0xFFCEFF

class Game
{
    public:
		Game();
        ~Game();
        void MountSaveData();
        Result Play();
		
		//Getters/setters
		u64 GetTitleId() { return TitleId; }
		void SetTitleId(u64 tid) { TitleId = tid; }
        void SetFlag(u8 flag) { Flag = flag; }
		u32 GetColor() { return SelColor; }
        void SetColor(u32 col) { SelColor = col; }

		SDL_Texture *Icon;
        SDL_Rect Pos;
        SDL_Texture *Sprite;
        std::string Text;
    private:
        u64 TitleId;
		u8 Flag;
		u32 SelColor;
};