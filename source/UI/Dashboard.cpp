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

#include "Dashboard.hpp"

Dashboard::Dashboard() {
    //
}

Dashboard::~Dashboard() {
	Buttons.clear();
}

void Dashboard::DrawWallpaper(std::string bgLay0, std::string bgLay1, std::string bgLay2, std::string dir, Renderer rend) {
    if(bgLay0 != "None") Draw::Texture(dir + bgLay0, 0, 0, rend);
	if(bgLay1 != "None") Draw::Texture(dir + bgLay1, 0, 0, rend);
	if(bgLay2 != "None") Draw::Texture(dir + bgLay2, 0, 0, rend);
}

void Dashboard::DrawButtons(Renderer rend) {
    for(auto const& button: Buttons) {
        if(button.Sprite != "")
            Draw::Texture(button.Sprite, button.X, button.Y, rend);
        else
            Draw::Rectangle(button.X, button.Y, button.W, button.H, {0xFF,0,0,0xFF}, rend);
    }
}

void Dashboard::AddButton(Button button) {
    Buttons.push_back(button);
}