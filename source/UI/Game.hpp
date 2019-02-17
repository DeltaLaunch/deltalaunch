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
#include <switch.h>
#include "Draw.hpp"
#include "Button.hpp"

class Game : public Button
{
    public:
        Game(u32 x, u32 y, Renderer *rend, u64 tid, u8 flag, std::function<Result()> callback);
		Game(u32 x, u32 y, u32 w, u32 h, u32 col, std::function<Result()> callback);
        ~Game();
		u64 TitleId;
		u8 Flag;
		SDL_Texture *Icon;
};