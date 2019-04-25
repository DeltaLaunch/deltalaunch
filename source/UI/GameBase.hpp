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

class GameBase
{
    public:
        GameBase() {
            Icon = nullptr;
            FolderID = 0;
        }
		GameBase(SDL_Texture *tex, u32 id) {
            Icon = tex;
            FolderID = id;
        };
        virtual ~GameBase() {};
        
        virtual Result Run() { return 0; };
        
        //Getters/setters
		virtual u64 GetTitleId() { return 0; }
		virtual void SetTitleId(u64 tid) {}
        virtual std::string GetName() { return Name; }
        virtual void SetName(std::string name) { Name = name; }
        virtual std::string GetAuthor() { return ""; }
        virtual void SetAuthor(std::string author) {}
        
        std::string Name;
        u32 FolderID;   //0 = not in folder; 1-n = folder id
        SDL_Rect Pos;
		SDL_Texture *Icon;
};