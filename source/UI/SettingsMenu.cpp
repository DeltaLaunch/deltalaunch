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

#include "SettingsMenu.hpp"

SettingsMenu::SettingsMenu(TTF_Font *fontHdr, TTF_Font *fontBdy, SDL_Rect pos) : Menu("Settings", fontHdr, fontBdy, pos){
    plInitialize();
	
	//vars
	menuOpt = 0;
    gameAreaType = GAMEAREA_MINI;
    gameSelectType = SELECT_OUTLINE;
}

SettingsMenu::~SettingsMenu() {
    plExit();
}

/*
*   Draw/Set Graphics
*/
void SettingsMenu::DrawPanel() {
	u32 panX = 500, panY = 100;
	switch(menuOpt){
		case 0:
		{
			Graphics::DrawText(FontBdy, panX, panY, "Toggle the lock screen flag.");
			break;
		}
		case 1:
		{
			Graphics::DrawText(FontBdy, panX, panY, "View internet settings.");
			break;
		}
		case 2:
		{
			Graphics::DrawText(FontBdy, panX, panY, "Edit profile.");
			break;
		}
        case 3:
        {
            Graphics::DrawText(FontBdy, panX, panY, "Change selection mode.");
            Graphics::DrawText(FontBdy, panX+5, panY+50, (gameSelectType == SELECT_OUTLINE ? "Mode: Outline" : "Mode: Diff size"));
            break;
        }
		case 4:
		{
			Graphics::DrawText(FontBdy, panX, panY, "Special specific information.");
			Graphics::DrawText(FontBdy, panX+5, panY+50, "Firmware: " + Settings::GetFirmwareVersion());
			Graphics::DrawText(FontBdy, panX+5, panY+70, "Serial: " + Settings::GetSerialNumber());
			break;
		}
	}
}

void SettingsMenu::DrawButtons() {
    int ind = 0;
    for(auto button: Buttons) {
        if(ind == menuOpt) {
            SDL_Rect pos; 
            pos.x = button->Pos.x-5; pos.y = button->Pos.y-5;
            pos.w = button->Pos.w+10; pos.h = button->Pos.h+10;
            Graphics::Rectangle(pos, AQUA);
            Graphics::Rectangle(button->Pos, GREY);
            Graphics::DrawText(FontBdy, button->Pos.x + 12, button->Pos.y + (button->Pos.h/2) - 4, button->Text, AQUA);
        }
        else {
            Graphics::DrawText(FontBdy, button->Pos.x + 12, button->Pos.y + (button->Pos.h/2) - 4, button->Text);
        }
        ind++;
    }
}

/*
*   Trigger events
*/
void SettingsMenu::Activate() {
    switch(menuOpt){
        case 0:
        {
            //toggle lock 
            break;
        }
        case 1:
        {
            //Internet settings
            break;
        }
        case 2:
        {
            //Profile
            break;
        }
        case 3:
        {
            //Selection type
            gameSelectType = (gameSelectType == SELECT_OUTLINE ? SELECT_SIZEDIFF : SELECT_OUTLINE);
            break;
        }
        case 4:
        {
            //system info/update
        }
    }
}

void SettingsMenu::Back() {
	if(currLayer == 0) Hide();
	else currLayer--;
}

void SettingsMenu::Update(u32 kDown) {
	if(kDown & KEY_A) Activate();
	if(kDown & KEY_B) Back();
	if(kDown & KEY_DUP) DecrementSelect();
	if(kDown & KEY_DDOWN) IncrementSelect();
    Graphics::RenderTexture(Sprite, Pos);
    Graphics::DrawText(FontHdr, 30, 25, GetTitle());
    DrawButtons();
	DrawPanel();
}