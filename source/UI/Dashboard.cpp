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

Dashboard::Dashboard(Renderer &rend, TTF_Font *small, TTF_Font *med, TTF_Font *big) {
    Rend = rend;
    smallFnt = small;
    medFnt = med;
    bigFnt = big;
    //Enable debug text by default
    dbg = new Debug(small, true);
}

Dashboard::~Dashboard() {
    delete dbg;
	Buttons.clear();
}

void Dashboard::DrawWallpaper(std::string bgLay0, std::string bgLay1, std::string bgLay2, std::string dir) {
    if(bgLay0 != "") Draw::Texture(dir + bgLay0, 0, 0, Rend);
	if(bgLay1 != "") Draw::Texture(dir + bgLay1, 0, 0, Rend);
	if(bgLay2 != "") Draw::Texture(dir + bgLay2, 0, 0, Rend);
}

void Dashboard::Update() {
	//Listen for touch presses
	for(auto &button: Buttons) {
        if(Hid::IsTouched(button.X, button.Y, button.X + button.W, button.Y + button.H)){
			lastErr = button.Run();
		}
    }
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

void Dashboard::AddButton(Button button) {
    Buttons.push_back(button);
}