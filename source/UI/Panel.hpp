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
#include <SDL2/SDL2_gfxPrimitives.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <switch.h>
#include <vector>
#include <tuple>
#include "../Core/Graphics.hpp"
#include "../Services/Hid.hpp"
#include "Button.hpp"
#include "Option.hpp"
#include "../Types.h"

class Panel
{
    public:
        Panel(TTF_Font *font, u32 x, u32 y);
        ~Panel();
        
        void AddString(u32 x, u32 y, std::string str) { Strings.push_back(std::tuple<u32, u32, std::string>(x,y,str)); }
        void AddOption(Option *op) { Options.push_back(op); }
        
        void Update(u32 kDown, bool selected);
        
        void IncrementSelect();
        void DecrementSelect();
        
        u32 OptionCnt() { return Options.size(); }
        
    private:
        SDL_Rect Pos;
        TTF_Font *Font;
        u32 optSelect;
        std::vector<Option*> Options;
        std::vector<std::tuple<u32, u32, std::string>> Strings;
};