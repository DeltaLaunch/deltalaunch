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

Button::Button(std::string sprite, uint32_t x, uint32_t y, std::function<void()> callback) {
	X = x;
	Y = y;
    Sprite = sprite;
    Callback = callback;
}

Button::Button(uint32_t x, uint32_t y, uint32_t w, uint32_t h, std::function<void()> callback) {
    X = x;
	Y = y;
    W = w;
    H = h;
	Callback = callback;
}

Button::~Button() {
	//
}

void Button::SetText(std::string txt) {
    Text = txt;
}

void Button::SetColor(SDL_Color col) {
    Color = col;
}

void Button::Run() {
	if(Callback != nullptr) Callback();
}