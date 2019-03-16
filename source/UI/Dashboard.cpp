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

Dashboard::Dashboard(Renderer *rend, u32 width, u32 height, std::string font) {
    Rend = rend;
    Width = width;
    Height = height;
    plInitialize();
    if(font == "") {
        PlFontData fnt;
        plGetSharedFontByType(&fnt, PlSharedFontType_Standard);
        debugFont = TTF_OpenFontRW(SDL_RWFromMem(fnt.address, fnt.size), 1, 14);
        hdrFont = TTF_OpenFontRW(SDL_RWFromMem(fnt.address, fnt.size), 1, 28);
        smallFont = TTF_OpenFontRW(SDL_RWFromMem(fnt.address, fnt.size), 1, 20);
    } 
    else {
        debugFont = TTF_OpenFont(font.c_str(), 14);
        hdrFont = TTF_OpenFont(font.c_str(), 28);
        smallFont = TTF_OpenFont(font.c_str(), 20);
    }
    lastErr = 0;
	selLayer = 0;
	gameSelectInd = 0;
    IsMenuOpen = false;
    debugInfo = false;
	MaxColumns = 12;
    Wallpaper = SDL_CreateTexture(Rend->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);
    LockScreen = SDL_CreateTexture(Rend->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);
    GameIconArea.x = 120; GameIconArea.y = 110;
    GameIconArea.w = 1080; GameIconArea.h = 450;
	selType == SELECT_OUTLINE;

    //appletGetPopFromGeneralChannelEvent(&h);
    //appletHolderWaitInteractiveOut(&h);
    //appletRequestForeground();
    
    //appletSetHandlesRequestToDisplay(true);
}

Dashboard::~Dashboard() {
    plExit();
    TTF_CloseFont(debugFont);
	TTF_CloseFont(hdrFont);
	TTF_CloseFont(smallFont);
	for(auto button: Buttons) delete button;
	for(auto menu: Menus) delete menu;
	for(auto game: Games) delete game;
    Buttons.clear();
	Menus.clear();
	Games.clear();
    SDL_DestroyTexture(Wallpaper);
    SDL_DestroyTexture(LockScreen);
	SDL_DestroyTexture(Battery);
}

/*
*   Draw/Set Graphics
*/
void Dashboard::DrawWallpaper() {
    SDL_Rect pos;
    pos.x = 0; pos.y = 0;
    pos.w = Width; pos.h = Height;
	Draw::RenderTexture(Wallpaper, pos, Rend);
}

void Dashboard::SetWallpaper(std::vector<std::string> layers) {
	SDL_Rect pos;
    pos.x = 0; pos.y = 0;
    pos.w = Width; pos.h = Height;
	SDL_Surface *wall = SDL_CreateRGBSurface(0,Width,Height,32,0,0,0,0);
	for(auto &l: layers) {
		SDL_Surface *lay = IMG_Load(l.c_str());
		SDL_BlitSurface(lay, &pos, wall, NULL);
		SDL_FreeSurface(lay);
	}
	Wallpaper = SDL_CreateTextureFromSurface(Rend->_renderer, wall);
    SDL_FreeSurface(wall);
}

void Dashboard::DrawLockScreen() {
    SDL_Rect pos;
    pos.x = 0; pos.y = 0;
    pos.w = Width; pos.h = Height;
	Draw::RenderTexture(LockScreen, pos, Rend);
}

void Dashboard::SetLockScreen(std::string image) {
    SDL_Surface *img = IMG_Load(image.c_str());
    LockScreen = SDL_CreateTextureFromSurface(Rend->_renderer, img);
    SDL_FreeSurface(img);
}

void Dashboard::DrawButtons() {
	int ind = 0;
    for(auto button: Buttons) {
		//Render icon/selected icon
        if(button->Sprite != nullptr) {
			if(ind == appletSelectInd && selLayer == 1)
				Draw::RenderTexture(button->SpriteSelect, button->Pos, Rend);
			else
				Draw::RenderTexture(button->Sprite, button->Pos, Rend);
		}
		//Default to rendering rectangle
        else
            Draw::Rectangle(button->Pos, button->Color, Rend);
        
		//Detect touch selection
        if(Hid::IsTouched(button->Pos) && !IsMenuOpen) {
            lastErr = button->Run();
			if(lastErr) App::ShowError("An Error has occurred!", "Error code: " + std::to_string(lastErr), lastErr);
		}
		ind++;
    }
}

void Dashboard::DrawGames() {
	int ind = 0;
	for(auto game: Games) {
        //Draw selection outline
        if(ind == gameSelectInd && selLayer == 0 && selType == SELECT_OUTLINE) {
            SDL_Rect pos = game->Pos; 
            pos.x -= 5; pos.y -= 5; pos.w += 10; pos.h += 10;
            Draw::Rectangle(pos, AQUA, Rend);
            if(pos.x < 0) OffsetGameIcons(1*(game->Pos.w + 14));
            if(pos.x + pos.w >= GameIconArea.x + GameIconArea.w) OffsetGameIcons(-1*(game->Pos.w + 14));
        }
		//Or draw size diff mode
		else if(ind == gameSelectInd && selLayer == 0 && selType == SELECT_SIZEDIFF) {
			SDL_Rect pos = game->Pos; 
            pos.x -= 10; pos.y -= 10; pos.w += 20; pos.h += 20;
			Draw::RenderTexture(game->Icon, pos, Rend);
            if(pos.x < 0) OffsetGameIcons(1*(game->Pos.w + 14));
            if(pos.x + pos.w >= GameIconArea.x + GameIconArea.w) OffsetGameIcons(-1*(game->Pos.w + 14));
		}
        //Draw either game icon or backer
		if(game->GetTitleId() != 0 && selType != SELECT_SIZEDIFF) {
			Draw::RenderTexture(game->Icon, game->Pos, Rend);
		} else {
			Draw::Rectangle(game->Pos, game->Color, Rend);
		}
        //Detect touch selection
        if(Hid::IsTouched(game->Pos) && !IsMenuOpen) {
            lastErr = game->Play();
			if(lastErr) App::ShowError("An Error has occurred!", "Error code: " + std::to_string(lastErr), lastErr);
            appletRequestForeground();
		}
		ind++;
	}
}

void Dashboard::SetGames() {
    //Create game images
	std::vector<u64> tids;
    App::GetTitleIds(tids);
    int i = 0;
	for(auto game: Games) {
        game->SetFlag(0);
        game->SetTitleId(tids[i]);
        NsApplicationControlData data = App::GetGameControlData(tids[i], 0);
        SDL_Surface *img = IMG_Load_RW(SDL_RWFromMem(data.icon, 0x20000), 1);
        if(game->Icon != nullptr)
            SDL_DestroyTexture(game->Icon);
        game->Pos.w = 256;
        game->Pos.h = 256;
		game->Icon = SDL_CreateTexture(Rend->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, game->Pos.w, game->Pos.h);
        if(img) 
            game->Icon = SDL_CreateTextureFromSurface(Rend->_renderer, img);
        SDL_FreeSurface(img);
        i++;
	}
    tids.clear();
}

void Dashboard::DrawOverlay() {
	Draw::RenderTexture(Battery, BatPos, Rend);
    Draw::Text(Rend, smallFont, ClkPos.x, ClkPos.y, Time::GetClock());
}

void Dashboard::SetOverlay(std::string battery, SDL_Rect batPos, SDL_Rect clkPos) {
    SDL_Surface *img = IMG_Load(battery.c_str());
    BatPos = batPos;
	BatPos.w = img->w; BatPos.h = img->h;
    ClkPos = clkPos;
	Battery = SDL_CreateTexture(Rend->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, img->w, img->h);
    Battery = SDL_CreateTextureFromSurface(Rend->_renderer, img);
    SDL_FreeSurface(img);
}

void Dashboard::DrawSettings(Menu *menu) {
	u32 panX = 500, panY = 100;
	switch(menu->GetSelection()){
		case 0:
		{
			Draw::Text(Rend, smallFont, panX, panY, "Toggle the lock screen flag.");
			break;
		}
		case 1:
		{
			Draw::Text(Rend, smallFont, panX, panY, "View internet settings.");
			break;
		}
		case 2:
		{
			Draw::Text(Rend, smallFont, panX, panY, "Edit profile.");
			break;
		}
		case 3:
		{
			Draw::Text(Rend, smallFont, panX, panY, "Special specific information.");
			Draw::Text(Rend, smallFont, panX+5, panY+50, "Firmware: " + Settings::GetFirmwareVersion());
			Draw::Text(Rend, smallFont, panX+5, panY+70, "Serial: " + Settings::GetSerialNumber());
			break;
		}
	}
}

void Dashboard::DrawMenus() {
    for(auto menu: Menus) {
		//Only draw menus that are open
        if(menu->IsOpen()) {
            if(menu->Sprite != nullptr)
				Draw::RenderTexture(menu->Sprite, menu->Pos, Rend);
			else
				Draw::Rectangle(menu->Pos, menu->Color, Rend);
			
			//Populate menu
			Draw::Text(Rend, hdrFont, 30, 25, menu->Title);
			int i = 0;
            for(auto button: menu->Buttons) {
				if(i == menu->GetSelection()) {
					SDL_Rect pos; 
					pos.x = button->Pos.x-5; pos.y = button->Pos.y-5;
					pos.w = button->Pos.w+10; pos.h = button->Pos.h+10;
					Draw::Rectangle(pos, AQUA, Rend);
                    Draw::Rectangle(button->Pos, GREY, Rend);
                    Draw::Text(Rend, smallFont, button->Pos.x + 12, button->Pos.y + (button->Pos.h/2) - 4, button->Text, AQUA);
				}
                else {
                    Draw::Text(Rend, smallFont, button->Pos.x + 12, button->Pos.y + (button->Pos.h/2) - 4, button->Text);
                }
                //Draw settings panel
                if(!menu->Title.compare("Settings")) {
					DrawSettings(menu);
                }
				i++;
            }
        }
    }
}

void Dashboard::DrawDebugText() {
    if(debugInfo && debugFont) {
        touchPosition touchPos;
        hidTouchRead(&touchPos, 0);
        u32 X = 14,  Y = 0, s = 14;
        Draw::Text(Rend, debugFont, X, Y+=s, "DeltaLaunch alpha!");
        Draw::Text(Rend, debugFont, X, Y+=s, "Firmware: " + Settings::GetFirmwareVersion());
        Draw::Text(Rend, debugFont, X, Y+=s, "Serial: " + Settings::GetSerialNumber());
		Draw::Text(Rend, debugFont, X, Y+=s, "Battery: " + std::to_string(Power::GetBatteryLife()) + "%");
        Draw::Text(Rend, debugFont, X, Y+=s, "Touch: X=" + std::to_string(touchPos.px) + "; y=" + std::to_string(touchPos.py));
        Y = 0;
    }
}

/*
*   Trigger events
*/
Result Dashboard::OpenMenu(std::string name) {
	for(auto menu: Menus) {
        if(menu->GetTitle() == name) {
			IsMenuOpen = true;;
			menu->Show();
		}
    }
    return 0;
}

Result Dashboard::CloseMenus() {
	for(auto menu: Menus) {
        if(menu->IsOpen()) {
            IsMenuOpen = false;
			menu->Hide();
		}
    }
    return 0;
}

void Dashboard::OffsetGameIcons(u32 deltaX) {
    int i = 0;
    for(auto game: Games) {
		game->Pos.x = (game->Pos.x <= 100+(i*(game->Pos.w+14))) ? (game->Pos.x + deltaX) : 100+(i*(game->Pos.w+14));
        i++;
	}
}

void Dashboard::IncrementMenuSel() {
	for(auto menu: Menus) {
        if(menu->IsOpen()) {
			if(menu->GetSelection() < menu->Buttons.size()-1)
				menu->IncrementSelect();
			else
				menu->SetSelection(0);
		}
	}
}

void Dashboard::DecrementMenuSel() {
	for(auto menu: Menus) {
        if(menu->GetSelection() > 0)
			menu->DecrementSelect();
		else
			menu->SetSelection(menu->Buttons.size()-1);
	}
}

void Dashboard::ActivateMenu() {
	for(auto menu: Menus) {
		if(menu->IsOpen()){
			if(menu->currLayer = 0) menu->currLayer++;
			else {
				//Settings
				if(!menu->Title.compare("Settings")) {
					switch(menu->GetSelection()){
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
							//system info/update
						}
					}
				}
			}
		}
	}
}

void Dashboard::DisengageMenu() {
	for(auto menu: Menus) {
		if(menu->IsOpen()) {
			if(menu->currLayer <= 0) CloseMenus();
			else menu->currLayer--;
		}
	}
}

void Dashboard::IncrementDashSel() {
	if(selLayer == 0 && gameSelectInd < Games.size()-1) gameSelectInd++;
	if(selLayer == 1 && appletSelectInd < Buttons.size()-1) appletSelectInd++;
}

void Dashboard::DecrementDashSel() {
	if(selLayer == 0 && gameSelectInd > 0) gameSelectInd--;
	if(selLayer == 1 && appletSelectInd > 0) appletSelectInd--;
}

void Dashboard::ActivateDash() {
    if(selLayer == 0) Games[gameSelectInd]->Play();
	if(selLayer == 1) Buttons[appletSelectInd]->Run();
}

/*
*   Add elements to form
*/
void Dashboard::AddButton(Button *button) {
    Buttons.push_back(button);
}

void Dashboard::AddGame(Game *game) {
	Games.push_back(game);
}

void Dashboard::AddMenu(Menu *menu) {
    Menus.push_back(menu);
}