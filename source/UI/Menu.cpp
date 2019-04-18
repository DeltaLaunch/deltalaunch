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

Menu::Menu(std::string title, SDL_Rect pos) {
	Title = title;
    Pos = pos;
    Visible = false;
    currLayer = 0;
    menuOpt = 0;
    Sprite = nullptr;
    running = true;
}

Menu::~Menu() {
    for(auto button: Buttons) {
        delete button;
    }
    Buttons.clear();
    SDL_DestroyTexture(Sprite);
}

void Menu::IncrementSelect() {
    if(menuOpt < Buttons.size()-1)
        menuOpt++;
    else
        menuOpt = 0;
}

void Menu::DecrementSelect() {
    if(menuOpt > 0)
        menuOpt--;
    else
        menuOpt = Buttons.size()-1;
}

void Menu::SetBackground(std::string tex) {
    SDL_Surface *img = IMG_Load(tex.c_str());
    Sprite = Graphics::CreateTexFromSurf(img);
    if(img) {
        Pos.w = img->w;
        Pos.h = img->h;
        SDL_FreeSurface(img);
    }
}