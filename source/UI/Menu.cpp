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

Menu::Menu(std::string title, std::string text, u32 x, u32 y, u32 w, u32 h, u32 col) {
	Title = title;
	Text = text;
	Visible = false;
	Pos.x = x; Pos.y = y;
	Pos.w = w; Pos.h = h;
	Color = col;
}

Menu::~Menu() {
	
}

void Menu::AddButton(Button button) {
	Buttons.push_back(button);
}

void Menu::Show() {
	Visible = true;
}

void Menu::Hide() {
	Visible = false;
}