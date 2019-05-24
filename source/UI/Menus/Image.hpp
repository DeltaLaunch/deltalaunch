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
#include "UIElement.hpp"

class Image: public UIElement
{
    public:
        Image(u32 x, u32 y, u32 w, u32 h, SDL_Texture *tex, std::function<Result()> callback = nullptr) : UIElement(x, y, w, h) {
            Tex = tex;
            Properties = ELEM_Image;
            Callback = callback;
        }
        
        ~Image() {
            SDL_DestroyTexture(Tex);
        };
        
        Result Run() override {
            return Callback();
        }
        
        void Draw(SDL_Rect pos, bool selected) override {
            SDL_Rect p = pos;
            p.x += Pos.x; p.y += Pos.y;
            p.w = Pos.w; p.h = Pos.h;
            if(selected){
                SDL_Rect ps = p;
                ps.x -= 5; ps.y -= 5;
                ps.w += 10; ps.h += 10;
                Graphics::Rectangle(ps, Graphics::GetDefaultSelCol());
            }
            Graphics::RenderTexture(Tex, p);
        }
        
        void SetImage(SDL_Texture *tex) override {
            if(Tex)
                SDL_DestroyTexture(Tex);
            Tex = tex;
        }
    private:
        SDL_Texture *Tex;
};