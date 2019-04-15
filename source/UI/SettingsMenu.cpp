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
    smallFont = fontBdy;
    panX = 500;
    panY = 100;
    u32 Y = 40, butW = 210, butH = 50, butCol = 0x202020FF, optW = 240;
    u32 space = 15+butH;
    
    Buttons.push_back(new Button("Lock Screen", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *lock = new Panel(fontBdy, panX, panY);
    lock->AddString(0, 0, std::string("Toggle the lock screen flag."));
    Panels.push_back(lock);
    
    Buttons.push_back(new Button("Internet", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *internet = new Panel(fontBdy, panX, panY);
    internet->AddString(0, 0, std::string("View internet settings."));
    Panels.push_back(internet);
    
    Buttons.push_back(new Button("Data Management", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *dataMan = new Panel(fontBdy, panX, panY);
    dataMan->AddString(0, 0, std::string("Manage your data."));
    Panels.push_back(dataMan);
    
    Buttons.push_back(new Button("Users", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *user = new Panel(fontBdy, panX, panY);
    user->AddString(0, 0, std::string("Edit user profiles."));
    Panels.push_back(user);
    
    Buttons.push_back(new Button("Look and Feel", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *look = new Panel(fontBdy, panX, panY);
    look->AddString(0, 0, std::string("Change look and feel."));
    const char *lookOps[] = {"test", "test2"};
    look->AddOption(new Option("Option:", lookOps, 0, 50, optW, butH, butCol, GameLookFeel));
    Panels.push_back(look);
    
    Buttons.push_back(new Button("Themes", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *theme = new Panel(fontBdy, panX, panY);
    theme->AddString(0, 0, std::string("Change current theme."));
    Panels.push_back(theme);
    
    Buttons.push_back(new Button("TV Settings", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *tvSet = new Panel(fontBdy, panX, panY);
    tvSet->AddString(0, 0, std::string("Manage your TV settings."));
    Panels.push_back(tvSet);
    
    Buttons.push_back(new Button("System Info", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *sysinfo = new Panel(fontBdy, panX, panY);
    sysinfo->AddString(0, 0, std::string("Special specific information."));
    sysinfo->AddString(5, 50, std::string("Firmware: " + Settings::GetFirmwareVersion()));
    sysinfo->AddString(5, 70, std::string("Serial: " + Settings::GetSerialNumber()));
    Panels.push_back(sysinfo);
}

SettingsMenu::~SettingsMenu() {
    plExit();
}

void SettingsMenu::GameLookFeel() {
    Settings::gameSelType = (Settings::gameSelType == SELECT_OUTLINE) ? SELECT_SIZEDIFF : SELECT_OUTLINE;
}

/*
*   Draw/Set Graphics
*/
void SettingsMenu::DrawButtons() {
    int ind = 0;
    for(auto button: Buttons) {
        Graphics::DrawButton(smallFont, button->Pos, button->Text, (!currLayer && (ind == menuOpt)) ? true : false);
        ind++;
    }
}

/*
*   Trigger events
*/
void SettingsMenu::Activate() {
    if(!currLayer && Panels[menuOpt]->OptionCnt() > 0) currLayer++;
}

void SettingsMenu::Back() {
	if(currLayer == 0) Hide();
	else currLayer--;
}

void SettingsMenu::Update(u32 kDown) {
	if(kDown & KEY_A) Activate();
	if(kDown & KEY_B) Back();
	if(kDown & KEY_DUP && !currLayer) DecrementSelect();
	if(kDown & KEY_DDOWN && !currLayer) IncrementSelect();
    Graphics::RenderTexture(Sprite, Pos);
    Graphics::DrawText(FontHdr, 30, 25, GetTitle());
    DrawButtons();
    Panels[menuOpt]->Update(kDown, (bool)currLayer);
}