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

SettingsMenu::SettingsMenu(SDL_Rect pos) : Menu("Settings", pos){
    plInitialize();
	menuOpt = 0;
}

SettingsMenu::~SettingsMenu() {
    for(auto pan: Panels) delete pan;
    plExit();
}

void SettingsMenu::Initialize() {
    panX = 500;
    panY = 100;
    u32 Y = 40, butW = 210, butH = 50, butCol = 0x202020FF, optW = 280, optY = 20;
    u32 space = 15+butH;
    
    //Toggle lock screen
    Buttons.push_back(new Button("Lock Screen", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *lock = new Panel(panX, panY);
    lock->AddString(0, 0, std::string("Toggle the lock screen flag."));
    std::vector<std::string> lck {"Off", "On"};
    lock->AddOption(new Option("Lock screen:", lck, 0, optY+=space, optW, butH, butCol, Settings::GetLockScreenFlag(), 
    []()->Result{ 
        bool l = Settings::GetLockScreenFlag();
        Settings::SetLockScreenFlag(!l);
        return 0;
    }));
    Panels.push_back(lock);
    
    //Internet/network settings
    Buttons.push_back(new Button("Internet", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *internet = new Panel(panX, panY);
    internet->AddString(0, 0, std::string("View internet settings."));
    Panels.push_back(internet);
    
    //Manage titles
    Buttons.push_back(new Button("Data Management", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *dataMan = new Panel(panX, panY);
    dataMan->AddString(0, 0, std::string("Manage your data."));
    Panels.push_back(dataMan);
    
    //User customization
    Buttons.push_back(new Button("Users", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *user = new Panel(panX, panY);
    user->AddString(0, 0, std::string("Edit user profiles."));
    std::vector<std::string> pfp {"Edit profile image"};
    user->AddImage(0, 40, 256, 256, Account::GetProfileImage(Account::GetFirstAccount()));
    user->AddOption(new Option("", pfp, 0, 260+space, 270, butH, butCol, NULL, 
    [user]()->Result{
        Result rc = 0;
        rc = Account::SetCustomProfileImage("/profile.jpg");
        user->SetImage(0, Account::GetProfileImage(Account::GetFirstAccount()));
        return rc;
    }));
    Panels.push_back(user);
    
    //Mechanics of how the UI works
    Buttons.push_back(new Button("Look and Feel", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *look = new Panel(panX, panY);
    look->AddString(0, 0, std::string("Change look and feel."));
    optY=20;
    std::vector<std::string> gamesel {"Outline", "Diffsize"};
    look->AddOption(new Option("Game select:", gamesel, 0, optY+=space, optW, butH, butCol, Settings::gameSelType, 
    []()->Result{
        Result rc = 0;
        Settings::gameSelType = (Settings::gameSelType == SELECT_OUTLINE) ? SELECT_SIZEDIFF : SELECT_OUTLINE; 
        return rc;
    }));
    std::vector<std::string> vrmode {"Disable", "Enable"};
    look->AddOption(new Option("VR Mode:", vrmode, 0, optY+=space, optW, butH, butCol, App::IsVrEnabled(), 
    []()->Result{
        Result rc = 0;
        bool b = App::IsVrEnabled();
        rc = appletSetVrModeEnabled(!b);
        if(!b) rc = appletBeginVrModeEx();
        else rc = appletEndVrModeEx();
        return rc;
    }));
    Panels.push_back(look);
    
    //Themes
    Buttons.push_back(new Button("Themes", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *theme = new Panel(panX, panY);
    theme->AddString(0, 0, std::string("Change current theme."));
    Panels.push_back(theme);
    
    //TV settings
    Buttons.push_back(new Button("TV Settings", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *tvSet = new Panel(panX, panY);
    tvSet->AddString(0, 0, std::string("Manage your TV settings."));
    Panels.push_back(tvSet);
    
    //System information and updates
    Buttons.push_back(new Button("System Info", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *sysinfo = new Panel(panX, panY);
    sysinfo->AddString(0, 0, std::string("Console specific information."));
    sysinfo->AddString(0, 50, std::string("Firmware: " + Settings::GetFirmwareVersion()));
    sysinfo->AddString(0, 75, std::string("Serial: " + Settings::GetSerialNumber()));
    std::vector<std::string> nick {Settings::GetDeviceNickname()};
    sysinfo->AddOption(new Option("Nickname:", nick, 0, 100, optW, butH, butCol, 0, 
    [sysinfo]()->Result{
        Result rc = 0;
        //
        return rc;
    }));
    std::vector<std::string> update {"Update"};
    sysinfo->AddOption(new Option("", update, 0, 400, 190, butH, butCol, 0, 
    []()->Result{
        Result rc = 0;
        //
        return rc;
    }));
    Panels.push_back(sysinfo);
}

/*
*   Draw/Set Graphics
*/
void SettingsMenu::DrawButtons() {
    int ind = 0;
    for(auto button: Buttons) {
        Graphics::DrawButton(button->Pos, button->Text, ((ind != menuOpt)) ? BTN_Unselected : (!currLayer ? BTN_Selected_Lay1 : BTN_Selected_Lay2));
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
	if(!currLayer) Hide();
	else currLayer--;
}

void SettingsMenu::Update(u32 kDown) {    
    Graphics::RenderTexture(Sprite, Pos);
    Graphics::DrawText(FNT_Big, 30, 25, GetTitle());
    DrawButtons();
    Panels[menuOpt]->Update(kDown, (bool)currLayer);
    if(kDown & KEY_A) Activate();
	if(kDown & KEY_B) Back();
	if(kDown & KEY_DUP && !currLayer) DecrementSelect();
	if(kDown & KEY_DDOWN && !currLayer) IncrementSelect();
}