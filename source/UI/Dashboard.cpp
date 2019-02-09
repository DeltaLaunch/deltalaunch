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

Dashboard::Dashboard(Renderer &rend, std::string fnt, u32 fntSize) {
    Rend = rend;
    smallFnt = TTF_OpenFont(fnt.c_str(), fntSize);
    //Enable debug text by default
    dbg = new Debug(smallFnt, true);
}

Dashboard::~Dashboard() {
    delete dbg;
	Buttons.clear();
}

void Dashboard::DrawWallpaper() {
    Draw::Texture(Lay0, 0, 0, Rend);
	Draw::Texture(Lay1, 0, 0, Rend);
	Draw::Texture(Lay2, 0, 0, Rend);
}

void Dashboard::DrawButtons() {
    for(auto &button: Buttons) {
        if(button.Sprite != "")
            Draw::Texture(button.Sprite, button.X, button.Y, Rend);
        else
            Draw::Rectangle(button.X, button.Y, button.W, button.H, button.Color, Rend);
    }
}

void Dashboard::DrawDebugText() {
    //Debug text
    touchPosition touchPos;
	hidTouchRead(&touchPos, 0);
    dbg->Print(Rend, "DeltaLaunch alpha!");
    dbg->Print(Rend, "Touch: X=" + std::to_string(touchPos.px) + "; y=" + std::to_string(touchPos.py));
    if(lastErr != 0) 
        dbg->Print(Rend, "Errors: " + std::to_string(lastErr));
    dbg->Clear();
}

void Dashboard::SetWallpaper(std::string lay0, std::string lay1, std::string lay2) {
    Lay0 = lay0;
    Lay1 = lay1;
    Lay2 = lay2;
}

void Dashboard::Update() {
	//Listen for touch presses
	for(auto &button: Buttons) {
        if(Hid::IsTouched(button.X, button.Y, button.X + button.W, button.Y + button.H)){
			lastErr = button.Run();
		}
    }
}

void Dashboard::AddButton(Button button) {
    Buttons.push_back(button);
}