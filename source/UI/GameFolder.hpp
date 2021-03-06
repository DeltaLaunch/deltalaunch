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
#include "GameBase.hpp"

class GameFolder: public GameBase
{
    public:
		GameFolder(SDL_Texture *tex, u32 id) : GameBase(tex, id) {
            Name = std::string("Folder ") + std::to_string(id);
        };
        
        ~GameFolder() {
            SDL_DestroyTexture(Icon);
        };
        
        void SetName(std::string name) {
            Name = name;
        }
        
        Result Run() {
            return 0;
        }
        
    private:
        std::function<void()> Callback;
};