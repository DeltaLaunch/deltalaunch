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
    u32 Y = 40, butW = 210, butH = 50, butCol = 0x202020FF, optW = 280, optY = 20;
    u32 space = 15+butH;
    
    Buttons.push_back(new Button("Lock Screen", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *lock = new Panel(fontBdy, panX, panY);
    lock->AddString(0, 0, std::string("Toggle the lock screen flag."));
    std::vector<std::string> lck {"Off", "On"};
    lock->AddOption(new Option("Lock screen:", lck, 0, optY+=space, optW, butH, butCol, LockScreenToggle, Settings::GetLockScreenFlag()));
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
    std::vector<std::string> pfp {"Edit profile image"};
    user->AddImage(0, 40, 256, 256, Account::GetProfileImage(Account::GetFirstAccount()));
    user->AddOption(new Option("", pfp, 0, 260+space, 270, butH, butCol, std::bind(EditProfilePic, user), NULL));
    Panels.push_back(user);
    
    Buttons.push_back(new Button("Look and Feel", 60, Y+=space, butW, butH, butCol, nullptr));
    Panel *look = new Panel(fontBdy, panX, panY);
    look->AddString(0, 0, std::string("Change look and feel."));
    optY=20;
    std::vector<std::string> gamesel {"Outline", "Diffsize"};
    look->AddOption(new Option("Game select:", gamesel, 0, optY+=space, optW, butH, butCol, GameLookFeel, Settings::gameSelType));
    std::vector<std::string> vrmode {"Disable", "Enable"};
    look->AddOption(new Option("VR Mode:", vrmode, 0, optY+=space, optW, butH, butCol, VrModeToggle, App::IsVrEnabled()));
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
    sysinfo->AddString(5, 90, std::string("Nickname: " + Settings::GetDeviceNickname()));
    std::vector<std::string> update {"Update"};
    sysinfo->AddOption(new Option("", update, 0, 400, 200, butH, butCol, UpdateConsole, 0));
    Panels.push_back(sysinfo);
}

SettingsMenu::~SettingsMenu() {
    for(auto pan: Panels) delete pan;
    plExit();
}

Result SettingsMenu::GameLookFeel() {
    Settings::gameSelType = (Settings::gameSelType == SELECT_OUTLINE) ? SELECT_SIZEDIFF : SELECT_OUTLINE;
}

Result SettingsMenu::LockScreenToggle() {
    bool l = Settings::GetLockScreenFlag();
    Settings::SetLockScreenFlag(!l);
}

Result SettingsMenu::VrModeToggle() {
    bool b = App::IsVrEnabled();
    appletSetVrModeEnabled(!b);
    if(!b) appletBeginVrModeEx();
    else appletEndVrModeEx();
    return 0;
}

Result SettingsMenu::UpdateConsole() {
    //
    return 0;
}

Result SettingsMenu::EditProfilePic(Panel *pan) {
    FILE *fp = fopen("/profile.jpg", "rb");
    Result rc = 0;
    if(fp){
        u128 uid = Account::GetFirstAccount();
        size_t fsize = 0;
        fseek(fp, 0, SEEK_END);
        fsize = ftell(fp);
        rewind(fp);
        u8 buf[fsize] = {0};
        fread(buf, fsize, 1, fp);
        fclose(fp);
        AccountProfile acc;
        AccountProfileBase pb;
        AccountUserData ud;
        accountInitialize();
        rc = accountGetProfile(&acc, uid);
        rc = accountProfileGet(&acc, &ud, &pb);
        ud.iconID = 0;
        rc = accGetProfileEditor(&acc, uid);
        rc = accStoreWithImage(&acc, &pb, &ud, buf, fsize);
        accountProfileClose(&acc);
        accountExit();
        pan->SetImage(0, Account::GetProfileImage(Account::GetFirstAccount()));
    }
    return rc;
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
    Graphics::RenderTexture(Sprite, Pos);
    Graphics::DrawText(FontHdr, 30, 25, GetTitle());
    DrawButtons();
    Panels[menuOpt]->Update(kDown, (bool)currLayer);
    if(kDown & KEY_A) Activate();
	if(kDown & KEY_B) Back();
	if(kDown & KEY_DUP && !currLayer) DecrementSelect();
	if(kDown & KEY_DDOWN && !currLayer) IncrementSelect();
}