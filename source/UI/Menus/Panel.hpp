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
#include "Button.hpp"
#include "Option.hpp"
#include "Image.hpp"
#include "PanelObjBase.hpp"
#include "../../Core/Graphics.hpp"
#include "../../Services/Hid.hpp"

class Panel
{
    public:
        Panel(u32 x, u32 y);
        ~Panel();
        
        void AddString(u32 x, u32 y, std::string str) { 
            Strings.push_back(std::tuple<u32, u32, std::string>(x,y,str)); 
        }
        void AddElement(PanelObjBase *op) { 
            Elements.push_back(op); 
        }
        
        void SetImage(u32 ind, SDL_Texture *tex) {
            u32 i = 0;
            for(auto elem: Elements) {
                if(elem->Properties == ELEM_Image) {
                    if(ind == i) {
                        SDL_DestroyTexture(elem->Tex);
                        elem->Tex = tex; 
                    }
                    i++;
                }
            }
        }
        
        void Update(u32 kDown, bool selected);
        
        void IncrementSelect();
        void DecrementSelect();
        
        u32 ElementCnt() { 
            return Elements.size();
        }
        
    private:
        SDL_Rect Pos;
        u32 optSelect;
        std::vector<PanelObjBase*> Elements;
        std::vector<std::tuple<u32, u32, std::string>> Strings;
};