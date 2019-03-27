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

Dashboard::Dashboard(u32 width, u32 height, std::string font) {
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
	
	//vars
    lastErr = 0;
	selLayer = 0;
	gameSelectInd = 0;
    msg = 0;
	gameRows = 1;
    debugInfo = false;
	MaxColumns = 12;
	IsMenuOpen = false;
    Wallpaper = SDL_CreateTexture(Graphics::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);
    LockScreen = SDL_CreateTexture(Graphics::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);
    GameIconArea.x = 120; GameIconArea.y = 110;
    GameIconArea.w = 1080; GameIconArea.h = 450;
	
	SetPos.x=SetPos.y=0; SetPos.w=Width; SetPos.h=Height;
	settings = new SettingsMenu(hdrFont, smallFont, SetPos);
	u32 Y = 20, butW = 200, butH = 60, butCol = 0x202020FF;
    u32 space = 20+butH;
    settings->AddButton(new Button("Lock Screen", 60, Y+=space, butW, butH, butCol, nullptr));
    settings->AddButton(new Button("Internet", 60, Y+=space, butW, butH, butCol, nullptr));
    settings->AddButton(new Button("Profile", 60, Y+=space, butW, butH, butCol, nullptr));
    settings->AddButton(new Button("Select Mode", 60, Y+=space, butW, butH, butCol, nullptr));
    settings->AddButton(new Button("System Info", 60, Y+=space, butW, butH, butCol, nullptr));
	settings->gameSelectType = SELECT_OUTLINE;

    appletRequestForeground();
    appletSetHandlesRequestToDisplay(true);
}

Dashboard::~Dashboard() {
    plExit();
    TTF_CloseFont(debugFont);
	TTF_CloseFont(hdrFont);
	TTF_CloseFont(smallFont);
	delete settings;
	for(auto button: Buttons) delete button;
	for(auto game: Games) delete game;
    Buttons.clear();
	Games.clear();
    SDL_DestroyTexture(Wallpaper);
    SDL_DestroyTexture(LockScreen);
	SDL_DestroyTexture(Battery);
}

/*
*   Draw/Set Graphics
*/
void Dashboard::UpdateDash(u32 kDown) {
    IsMenuOpen = settings->IsOpen();
    
    if(kDown & KEY_A) ActivateDash();
	if(kDown & KEY_PLUS) ToggleDebug();
	if(kDown & KEY_MINUS) ;
	if(kDown & KEY_DLEFT) DecrementDashSel();
	if(kDown & KEY_DRIGHT) IncrementDashSel();
	if(kDown & KEY_DUP) selLayer = 0;
	if(kDown & KEY_DDOWN) selLayer = 1;
	if(Hid::IsTouched(GameIconArea) && !Hid::IsTapped(GameIconArea)) {
		if(lastPosX != 0) 
			OffsetGameIcons(Hid::GetTouchPos().px - lastPosX);
		lastPosX = Hid::GetTouchPos().px;
	} else {
		lastPosX = 0;
	}
    
    if(App::IsGamecardInserted() == GcState) {
        SetGames();
        GcState = !GcState;
    }
}

void Dashboard::DrawWallpaper() {
	IsMenuOpen = settings->IsOpen();
    SDL_Rect pos;
    pos.x = 0; pos.y = 0;
    pos.w = Width; pos.h = Height;
	Graphics::RenderTexture(Wallpaper, pos);
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
	Wallpaper = SDL_CreateTextureFromSurface(Graphics::GetRenderer(), wall);
    SDL_FreeSurface(wall);
}

void Dashboard::DrawLockScreen() {
    SDL_Rect pos;
    pos.x = 0; pos.y = 0;
    pos.w = Width; pos.h = Height;
	Graphics::RenderTexture(LockScreen, pos);
}

void Dashboard::SetLockScreen(std::string image) {
    SDL_Surface *img = IMG_Load(image.c_str());
    LockScreen = SDL_CreateTextureFromSurface(Graphics::GetRenderer(), img);
    SDL_FreeSurface(img);
}

void Dashboard::DrawButtons() {
	int ind = 0;
    for(auto button: Buttons) {
		//Render icon/selected icon
        if(button->Sprite != nullptr) {
			if(ind == appletSelectInd && selLayer == 1)
				Graphics::RenderTexture(button->SpriteSelect, button->Pos);
			else
				Graphics::RenderTexture(button->Sprite, button->Pos);
		}
		//Default to rendering rectangle
        else
            Graphics::Rectangle(button->Pos, button->Color);
        
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
        //Classic outline format
        if(settings->gameSelectType == SELECT_OUTLINE) {
            //Draw selection outline
            if(ind == gameSelectInd && selLayer == 0) {
                SDL_Rect pos = game->Pos; 
                pos.x -= 5; pos.y -= 5; pos.w += 10; pos.h += 10;
                Graphics::Rectangle(pos, game->GetColor());
            }
            
            //Draw either game icon or backer
            if(game->GetTitleId() != 0) {
                Graphics::RenderTexture(game->Icon, game->Pos);
            } else {
                Graphics::Rectangle(game->Pos, 0x70);
            }
        }
		//Or draw size diff mode
		if(settings->gameSelectType == SELECT_SIZEDIFF) {
            //Draw game bigger
            if(ind == gameSelectInd && selLayer == 0) {
                SDL_Rect pos = game->Pos; 
                pos.x -= 20; pos.y -= 20; pos.w += 40; pos.h += 40;
                Graphics::RenderTexture(game->Icon, pos);
            }
            else {
                //Draw either game icon or backer
                if(game->GetTitleId() != 0) {
                    Graphics::RenderTexture(game->Icon, game->Pos);
                }
                Graphics::Rectangle(game->Pos, 0x70);
            }
		}
        
        //Detect touch selection
        if(Hid::IsTouched(game->Pos) && !IsMenuOpen && game->GetTitleId()) {
			lastErr = game->Play();
			if(lastErr) 
				App::ShowError("An Error has occurred!", "Error code: " + std::to_string(lastErr), lastErr);
		}
		ind++;
	}
}

void Dashboard::SetGames() {
    //Create game images
	std::vector<u64> tids;
    App::GetTitleIds(tids);
    int i = 0;
	size_t total = tids.size();
	for(auto game: Games) {
		u64 tid = i < total ? tids[i] : 0;
        game->SetFlag(0);
        game->SetTitleId(tid);
        game->Pos.w = 256/gameRows; 
        game->Pos.h = 256/gameRows;
		game->Pos.x = 100+((i%(MaxColumns/gameRows))*(game->Pos.w+(14/gameRows))); 
        game->Pos.y = 200+((i/(MaxColumns/gameRows))*(game->Pos.h+(14/gameRows)));
		if(tid) {
			NsApplicationControlData data = App::GetGameControlData(tid, 0);
			SDL_Surface *img = IMG_Load_RW(SDL_RWFromMem(data.icon, 0x20000), 1);
			if(game->Icon != nullptr)
				SDL_DestroyTexture(game->Icon);
			game->Icon = SDL_CreateTexture(Graphics::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, game->Pos.w, game->Pos.h);
			if(img) 
				game->Icon = SDL_CreateTextureFromSurface(Graphics::GetRenderer(), img);
			SDL_FreeSurface(img);
		}
        i++;
	}
    tids.clear();
}

void Dashboard::DrawOverlay() {
	Graphics::RenderTexture(Battery, BatPos);
    Graphics::DrawText(smallFont, ClkPos.x, ClkPos.y, Time::GetClock());
}

void Dashboard::SetOverlay(std::string battery, SDL_Rect batPos, SDL_Rect clkPos) {
    SDL_Surface *img = IMG_Load(battery.c_str());
    BatPos = batPos;
	BatPos.w = img->w; BatPos.h = img->h;
    ClkPos = clkPos;
	Battery = SDL_CreateTexture(Graphics::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, img->w, img->h);
    Battery = SDL_CreateTextureFromSurface(Graphics::GetRenderer(), img);
    SDL_FreeSurface(img);
}

void Dashboard::DrawDebugText() {
    if(debugInfo && debugFont) {
        touchPosition touchPos;
        hidTouchRead(&touchPos, 0);
        u32 X = 14,  Y = 0, s = 14;
        Graphics::DrawText(debugFont, X, Y+=s, "DeltaLaunch alpha!");
        Graphics::DrawText(debugFont, X, Y+=s, "Firmware: " + Settings::GetFirmwareVersion());
        Graphics::DrawText(debugFont, X, Y+=s, "Serial: " + Settings::GetSerialNumber());
		Graphics::DrawText(debugFont, X, Y+=s, "Battery: " + std::to_string(Power::GetBatteryLife()) + "%");
        Graphics::DrawText(debugFont, X, Y+=s, "Touch: X=" + std::to_string(touchPos.px) + "; y=" + std::to_string(touchPos.py));
        Graphics::DrawText(debugFont, X, Y+=s, "Message: " + std::to_string(msg));
        Y = 0;
    }
}

/*
*   Trigger events
*/
void Dashboard::OffsetGameIcons(u32 deltaX) {
    int i = 0;
    for(auto game: Games) {
		game->Pos.x = (game->Pos.x <= 100+(i*(game->Pos.w+(14/gameRows)))) ? (game->Pos.x + deltaX) : 100+(i*(game->Pos.w+(14/gameRows)));
        i++;
	}
}

void Dashboard::IncrementDashSel() {
	if(selLayer == 0 && gameSelectInd < Games.size()-1) {
        gameSelectInd++;
        if(Games[gameSelectInd]->Pos.x < 0) 
            OffsetGameIcons(100 - (Games[gameSelectInd]->Pos.x));
        if(Games[gameSelectInd]->Pos.x + Games[gameSelectInd]->Pos.w >= GameIconArea.x + GameIconArea.w) 
            OffsetGameIcons((GameIconArea.x + GameIconArea.w) - (Games[gameSelectInd]->Pos.x + Games[gameSelectInd]->Pos.w));
    }
	if(selLayer == 1 && appletSelectInd < Buttons.size()-1) appletSelectInd++;
}

void Dashboard::DecrementDashSel() {
	if(selLayer == 0 && gameSelectInd > 0) {
        gameSelectInd--;
        if(Games[gameSelectInd]->Pos.x < 0) 
            OffsetGameIcons(100 - (Games[gameSelectInd]->Pos.x));
        if(Games[gameSelectInd]->Pos.x + Games[gameSelectInd]->Pos.w >= GameIconArea.x + GameIconArea.w) 
            OffsetGameIcons((GameIconArea.x + GameIconArea.w) - (Games[gameSelectInd]->Pos.x + Games[gameSelectInd]->Pos.w));
    }
	if(selLayer == 1 && appletSelectInd > 0) appletSelectInd--;
}

void Dashboard::ActivateDash() {
    if(selLayer == 0) Games[gameSelectInd]->Play();
	if(selLayer == 1) Buttons[appletSelectInd]->Run();
}

Result Dashboard::OpenSettings() {
	IsMenuOpen = true;
	settings->Show();
	return 0;
}

Result Dashboard::CloseSettings() {
	IsMenuOpen = false;
	settings->Hide();
	return 0;
}

void Dashboard::UpdateSettings(u32 hid) {
	settings->Update(hid);
    IsMenuOpen = settings->IsOpen();
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