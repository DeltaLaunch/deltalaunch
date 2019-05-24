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
#include "UIElement.hpp"
#include "../../Core/Graphics.hpp"

class Button: public UIElement
{
    public:
        Button(std::string sprite, std::string spriteSel, u32 x, u32 y, std::function<Result()> callback = nullptr) : UIElement(x, y, 0, 0) {
            Pos.x = x; Pos.y = y;
            Callback = callback;
            Properties = ELEM_Button;
            Sprite = nullptr;
            if(sprite != "") {
                SDL_Surface *img = IMG_Load(sprite.c_str());
                Sprite = Graphics::CreateTexFromSurf(img);
                if(img) {
                    Pos.w = img->w;
                    Pos.h = img->h;
                    SDL_FreeSurface(img);
                }
            }
            SpriteSelect = nullptr;
            if(spriteSel != "") {
                SDL_Surface *img = IMG_Load(spriteSel.c_str());
                SpriteSelect = Graphics::CreateTexFromSurf(img);
                if(img) {
                    SDL_FreeSurface(img);
                }
            }
        }
        
        Button(std::string text, u32 x, u32 y, u32 w, u32 h, u32 col, std::function<Result()> callback = nullptr) : UIElement(x, y, w, h) {
            Pos.x = x; Pos.y = y;
            Pos.w = w; Pos.h = h;
            Callback = callback;
            Properties = ELEM_Button;
            Color = col;
            Sprite = nullptr;
            SpriteSelect = nullptr;
            Text = text;
        }
        
        ~Button() {
            if(Sprite) SDL_DestroyTexture(Sprite);
            if(SpriteSelect) SDL_DestroyTexture(SpriteSelect);
        }
        
        Result Run() override {
            Result res = 0;
            if(Callback != nullptr) res = Callback();
            return res;
        }
        
        //vars
        SDL_Texture *Sprite;
		SDL_Texture *SpriteSelect;
        u32 Color;
};