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

#include "Button.hpp"

Button::Button(std::string sprite, u32 x, u32 y, Renderer *rend, std::function<Result()> callback) {
    Pos.x = x; Pos.y = y;
    Callback = callback;
    Sprite = nullptr;
	if(sprite != "") {
		SDL_Surface *img = IMG_Load(sprite.c_str());
		Sprite = Draw::CreateTexFromSurf(img, rend);
		if(img) {
			Pos.w = img->w;
			Pos.h = img->h;
			SDL_FreeSurface(img);
		}
	}
}

Button::Button(u32 x, u32 y, u32 w, u32 h, u32 col, std::function<Result()> callback) {
    Pos.x = x; Pos.y = y;
    Pos.w = w; Pos.h = h;
    Callback = callback;
    Color = col;
	Sprite = nullptr;
}

Button::~Button() {
    SDL_DestroyTexture(Sprite);
}

void Button::SetText(std::string txt) {
    Text = txt;
}

Result Button::Run() {
    Result res = 0;
    if(Callback != nullptr) res = Callback();
    return res;
}