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
	//Panel location
    panX = 500;
    panY = 100;
}

SettingsMenu::~SettingsMenu() {
    //
}

void SettingsMenu::Initialize() {
    u32 Y = 40, butW = 280, butH = 50, butCol = Graphics::GetDefaultSelCol(), optW = 350, optY = 20;
    u32 space = 15+butH;
    
    //Toggle lock screen
    MenuElements.push_back(new Button("Lock Screen", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *lock = new Panel(panX, panY);
    lock->AddString(0, 0, std::string("Toggle the lock screen flag."));
    std::vector<std::string> lck {"Off", "On"};
    lock->AddElement(new Option("Lock screen:", lck, 0, optY+=space, optW, butH, butCol, Settings::GetLockScreenFlag(), 
    []()->Result{ 
        bool l = Settings::GetLockScreenFlag();
        Settings::SetLockScreenFlag(!l);
        return 0;
    }));
    Panels.push_back(lock);
    
    //Internet/network settings
    optY = 20;
    MenuElements.push_back(new Button("Internet", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *internet = new Panel(panX, panY);
    internet->AddString(0, 0, std::string("View internet settings."));
    internet->AddElement(new Option("Internet settings", "test", 0, optY+=space, optW, butH, butCol, 0, 
    []()->Result{
        Result rc = 0;
        rc = App::LaunchNetConnect();
        return rc;
    }));
    Panels.push_back(internet);
    
    //Manage titles
    MenuElements.push_back(new Button("Data Management", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *dataMan = new Panel(panX, panY);
    dataMan->AddString(0, 0, std::string("Manage your data."));
    Panels.push_back(dataMan);
    
    //User customization
    MenuElements.push_back(new Button("Users", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *user = new Panel(panX, panY);
    user->AddString(0, 0, std::string("Edit user profiles."));
    user->AddElement(new Image(0, 40, 256, 256, Account::GetProfileImage(Account::GetFirstAccount()), 
    [user]()->Result{
        Result rc = 0;
        rc = Account::SetCustomProfileImage("/profile.jpg");
        user->SetImage(0, Account::GetProfileImage(Account::GetFirstAccount()));
        return rc;
    }));
    std::vector<std::string> pfp {"Use pre-defined image"};
    user->AddElement(new Option("", pfp, 0, 260+space, optW, butH, butCol, 0, 
    [user]()->Result{
        Result rc = 0;
        //non-custom pfp
        return rc;
    }));
    Panels.push_back(user);
    
    //Mechanics of how the UI works
    MenuElements.push_back(new Button("Look and Feel", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *look = new Panel(panX, panY);
    look->AddString(0, 0, std::string("Change look and feel."));
    optY=20;
    std::vector<std::string> gamesel {"Outline", "Diffsize", "Ps4 style"};
    look->AddElement(new Option("Game select:", gamesel, 0, optY+=space, optW, butH, butCol, Settings::gameSelType, 
    []()->Result{
        Result rc = 0;
        Settings::gameSelType = ((int)(Settings::gameSelType + 1) > 2) ? (SelectType)0 : (SelectType)(Settings::gameSelType + 1); 
        return rc;
    }));
    std::vector<std::string> vrmode {"Disable", "Enable"};
    look->AddElement(new Option("VR Mode:", vrmode, 0, optY+=space, optW, butH, butCol, App::IsVrEnabled(), 
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
    MenuElements.push_back(new Button("Themes", 60, Y+=space, butW, butH, butCol));
    Panel *theme = new Panel(panX, panY);
    theme->AddString(0, 0, std::string("Change current theme."));
    Panels.push_back(theme);
    
    //TV settings
    MenuElements.push_back(new Button("TV Settings", 60, Y+=space, butW, butH, butCol));
    Panel *tvSet = new Panel(panX, panY);
    tvSet->AddString(0, 0, std::string("Manage your TV settings."));
    Panels.push_back(tvSet);
    
    //System information and updates
    MenuElements.push_back(new Button("System Info", 60, Y+=space, butW, butH, butCol));
    Panel *sysinfo = new Panel(panX, panY);
    sysinfo->AddString(0, 0, std::string("Console specific information."));
    sysinfo->AddString(0, 50, std::string("Firmware: " + Settings::GetFirmwareVersion()));
    sysinfo->AddString(0, 75, std::string("Serial: " + Settings::GetSerialNumber()));
    Y = 100;
    sysinfo->AddElement(new Option("Nickname:", Settings::GetDeviceNickname(), 0, Y+=space, optW, butH, butCol, 0, 
    [sysinfo]()->Result{
        Result rc = 0;
        char out[0xC00] = {0};
        App::LaunchSwkbd(out, "Nickname", "", "Set name", "");
        std::string name = std::string(out);
        //Settings::SetDeviceNickname(name);
        sysinfo->SetOptText(0, name);
        return rc;
    }));
    sysinfo->AddElement(new Option("", "Config controllers", 0, Y+=space, optW, butH, butCol, 0, 
    []()->Result{
        Result rc = 0;
        rc = App::LaunchController();
        return rc;
    }));
    sysinfo->AddElement(new Option("", "Update", 0, 400, optW, butH, butCol, 0, 
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
    for(auto button: MenuElements) {
        Graphics::DrawButton(button->Pos, button->Text, ((ind != menuOpt)) ? BTN_Unselected : (!currLayer ? BTN_Selected_Lay1 : BTN_Selected_Lay2));
        ind++;
    }
}

/*
*   Trigger events
*/
void SettingsMenu::Update() {
    if(!IsOpen()) return;
    Graphics::RenderTexture(Sprite, Pos);
    Graphics::DrawText(Fonts::FONT_LARGE, 30, 25, GetTitle());
    DrawButtons();
    Panels[menuOpt]->Update((bool)currLayer);
    if(Hid::Input & KEY_A) Activate();
	if(Hid::Input & KEY_DUP && !currLayer) DecrementSelect();
	if(Hid::Input & KEY_DDOWN && !currLayer) IncrementSelect();
    if(Hid::Input & KEY_B) Back();
}