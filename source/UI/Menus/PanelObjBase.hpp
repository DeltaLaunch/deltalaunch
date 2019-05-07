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
#include <switch.h>
#include <functional>

enum PanelElement {
    ELEM_Image,
    ELEM_Button,
    ELEM_Option
};

class PanelObjBase
{
    public:
        PanelObjBase(u32 x, u32 y, u32 w, u32 h) {
            Pos.x = x;
            Pos.y = y;
            Pos.w = w;
            Pos.h = h;
        }
        virtual ~PanelObjBase() {}
        
        virtual void Draw(SDL_Rect pos, bool selected) {}
        virtual void Run() {}
        
        //Setters for panel elements
        virtual void SetOption(u32 ind, std::string text) {}
        virtual void SetOptions(std::vector<std::string> opts) {}
        virtual void SetImage(SDL_Texture *tex) {}
        
        bool HasFunc() { return Callback != nullptr; }
        
        PanelElement Properties;
        
    protected:
        SDL_Rect Pos;
        std::function<Result()> Callback;
};