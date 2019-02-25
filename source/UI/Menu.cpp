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

#include "Menu.hpp"

Menu::Menu(std::string title, std::string text, u32 x, u32 y, std::string texture, Renderer *rend) {
	Title = title;
	Text = text;
	Visible = false;
	currLayer = 0;
	SelectPos = 0;
	Pos.x = x; Pos.y = y;
	Sprite = nullptr;
	SDL_Surface *img = IMG_Load(texture.c_str());
    Sprite = Draw::CreateTexFromSurf(img, rend);
    if(img) {
        Pos.w = img->w;
        Pos.h = img->h;
        SDL_FreeSurface(img);
    }
}

Menu::~Menu() {
	for(auto button: Buttons) {
        delete button;
    }
    Buttons.clear();
	SDL_DestroyTexture(Sprite);
}

void Menu::AddButton(Button *button) {
	Buttons.push_back(button);
}

void Menu::Show() {
	Visible = true;
}

void Menu::Hide() {
	Visible = false;
}

void Menu::Run(u32 index) {
	
}