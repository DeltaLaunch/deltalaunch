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

Dashboard::Dashboard(u32 width, u32 height) {
    Width = width;
    Height = height;
	
	//vars
    lastErr = 0;
	App::dashLayer = 0;
	App::gameSelectInd = 0;
    Settings::gameSelType = SELECT_OUTLINE;
    Settings::gameAreaType = GAMEAREA_FULL;
	gameRows = 1;
    debugInfo = false;
	MaxColumns = 12;
    Wallpaper = SDL_CreateTexture(Graphics::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);
    LockScreen = SDL_CreateTexture(Graphics::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);
	
	SetPos.x=SetPos.y=0; SetPos.w=Width; SetPos.h=Height;
	settings = new SettingsMenu(SetPos);
    settings->Initialize();
}

Dashboard::~Dashboard() {
	delete settings;
	for(auto button: Buttons) delete button;
	for(auto game: Games) delete game;
    Buttons.clear();
	Games.clear();
    SDL_DestroyTexture(Wallpaper);
    SDL_DestroyTexture(LockScreen);
	SDL_DestroyTexture(Battery);
}

void Dashboard::Initialize() {
    //Create buttons to add to dash
    unsigned x = 230;       //padding on edges
    unsigned space = 100;   //space inbetween
    std::vector<std::string> names = Settings::GetThemeNames();
    std::string baseThemeDir = names[0] + std::string("/");
    //FILE *fp = fopen("test.txt", "w"); fwrite(baseThemeDir.c_str(), strlen(baseThemeDir.c_str()), 1, fp); fclose(fp);
    INIReader cfg(names[0] + ".cfg");
    std::vector<std::tuple<std::string, std::function<Result()>>> ButtonFuncs{
        std::tuple<std::string, std::function<Result()>>{"WebButton", std::bind(App::LaunchWebsite, "https://google.com/")},
        std::tuple<std::string, std::function<Result()>>{"NewsButton", nullptr},
        std::tuple<std::string, std::function<Result()>>{"ShopButton", App::LaunchShop},
        std::tuple<std::string, std::function<Result()>>{"AlbumButton", std::bind(App::LaunchAlbum, 1, false)}, //1,false | 2,true
        std::tuple<std::string, std::function<Result()>>{"HomebrewButton", App::LaunchHbl},
        std::tuple<std::string, std::function<Result()>>{"SettingsButton", std::bind(&Dashboard::OpenSettings, this)},
        std::tuple<std::string, std::function<Result()>>{"SleepButton", Power::Reboot},
    };
    for(u32 but = 0; but < ButtonFuncs.size(); but++) {
        Buttons.push_back(new Button(
            baseThemeDir + cfg.Get(std::get<0>(ButtonFuncs.at(but)), "sprite", "romfs:/Graphics/Icons/" + std::get<0>(ButtonFuncs.at(but)) + ".png"), 
            baseThemeDir + cfg.Get(std::get<0>(ButtonFuncs.at(but)), "sprite_select", "romfs:/Graphics/Icons/" + std::get<0>(ButtonFuncs.at(but)) + "_select.png"), 
            cfg.GetInteger(std::get<0>(ButtonFuncs.at(but)), "x", x+=space), 
            cfg.GetInteger(std::get<0>(ButtonFuncs.at(but)), "y", 600), 
            std::get<1>(ButtonFuncs.at(but))
        ));
    }
    
    //Boundries: (120, 110), (1200, 560) .. 450px vert
    for(u32 i = 0; i < MaxColumns; i++){
		Games.push_back(new Game());
	}
    SetGames();
}

/*
*   Draw/Set Graphics
*/
void Dashboard::UpdateDash(u32 kDown) {    
    if(kDown & KEY_A) ActivateDash();
	if(kDown & KEY_LSTICK)  debugInfo = !debugInfo;
	if(kDown & KEY_MINUS) ;
	if((kDown & KEY_DLEFT) || (kDown & KEY_LSTICK_LEFT)) DecrementDashSel();
	if((kDown & KEY_DRIGHT) || (kDown & KEY_LSTICK_RIGHT)) IncrementDashSel();
	if((kDown & KEY_DUP) || (kDown & KEY_LSTICK_UP)) App::dashLayer = 0;
	if((kDown & KEY_DDOWN) || (kDown & KEY_LSTICK_DOWN)) App::dashLayer = 1;
	if(Hid::IsTouched(GameIconArea)) {
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
	unsigned ind = 0;
    for(auto button: Buttons) {
		//Render icon/selected icon
        if(button->Sprite != nullptr) {
			if(ind == App::appletSelectInd && App::dashLayer == 1)
				Graphics::RenderTexture(button->SpriteSelect, button->Pos);
			else
				Graphics::RenderTexture(button->Sprite, button->Pos);
		}
		//Default to rendering rectangle
        else
            Graphics::Rectangle(button->Pos, button->Color);
        
		//Detect touch selection
        if(Hid::IsTouched(button->Pos) && !settings->IsOpen()) {
            lastErr = button->Run();
			if(lastErr) App::ShowError("An Error has occurred!", "Error code: " + std::to_string(lastErr), lastErr);
		}
		ind++;
    }
}

void Dashboard::DrawGames() {
	unsigned ind = 0;
	for(auto game: Games) {
        //Classic outline format
        if(Settings::gameSelType == SELECT_OUTLINE) {
            //Draw selection outline
            if(ind == App::gameSelectInd && App::dashLayer == 0) {
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
		if(Settings::gameSelType == SELECT_SIZEDIFF) {
            //Draw game bigger
            if(ind == App::gameSelectInd && App::dashLayer == 0) {
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
        if(!settings->IsOpen() && game->GetTitleId() && Hid::IsTouched(game->Pos)) {
            if (ind == App::gameSelectInd) {
                lastErr = game->Play();
                if(lastErr) 
                    App::ShowError("An Error has occurred!", "Error code: " + std::to_string(lastErr), lastErr);
            } else {
                App::gameSelectInd = ind;
            }
		}
		ind++;
	}
}

void Dashboard::SetGames() {
    GameIconArea.x = 120; GameIconArea.y = 110;
    GameIconArea.w = 1080; GameIconArea.h = 450;
    u32 normalPortraitSize = 256;
    
    //Create game images
	std::vector<u64> tids;
    App::GetTitleIds(tids);
    int i = 0;
	size_t total = tids.size();
	for(auto game: Games) {
		u64 tid = i < (int)total ? tids[i] : 0;
        game->SetFlag(0);
        game->SetTitleId(tid);
        game->Pos.w = normalPortraitSize/gameRows; 
        game->Pos.h = normalPortraitSize/gameRows;
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
    Graphics::DrawText(FNT_Small, ClkPos.x, ClkPos.y, Time::GetClock());
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
    if(debugInfo) {
        touchPosition touchPos;
        hidTouchRead(&touchPos, 0);
        u32 X = 14,  Y = 0, s = 14;
        Graphics::DrawText(FNT_Debug, X, Y+=s, "DeltaLaunch alpha!");
        Graphics::DrawText(FNT_Debug, X, Y+=s, "Firmware: " + Settings::GetFirmwareVersion());
        Graphics::DrawText(FNT_Debug, X, Y+=s, "Serial: " + Settings::GetSerialNumber());
		Graphics::DrawText(FNT_Debug, X, Y+=s, "Battery: " + std::to_string(Power::GetBatteryLife()) + "%");
        Graphics::DrawText(FNT_Debug, X, Y+=s, "Touch: X=" + std::to_string(touchPos.px) + "; y=" + std::to_string(touchPos.py));
        Graphics::DrawText(FNT_Debug, X, Y+=s, "Ae Message: " + std::to_string(App::lastAeCmd));
		Graphics::DrawText(FNT_Debug, X, Y+=s, "Sams Message: " + std::to_string(App::lastSamsCmd));
        Graphics::DrawText(FNT_Debug, X, Y+=s, "Last Error: " + std::to_string(lastErr));
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
	if(App::dashLayer == 0 && App::gameSelectInd < Games.size()-1) {
        App::gameSelectInd++;
        if(Games[App::gameSelectInd]->Pos.x < 0) 
            OffsetGameIcons(100 - (Games[App::gameSelectInd]->Pos.x));
        if(Games[App::gameSelectInd]->Pos.x + Games[App::gameSelectInd]->Pos.w >= GameIconArea.x + GameIconArea.w) 
            OffsetGameIcons((GameIconArea.x + GameIconArea.w) - (Games[App::gameSelectInd]->Pos.x + Games[App::gameSelectInd]->Pos.w));
    }
	if(App::dashLayer == 1 && App::appletSelectInd < Buttons.size()-1) App::appletSelectInd++;
}

void Dashboard::DecrementDashSel() {
	if(App::dashLayer == 0 && App::gameSelectInd > 0) {
        App::gameSelectInd--;
        if(Games[App::gameSelectInd]->Pos.x < 0) 
            OffsetGameIcons(100 - (Games[App::gameSelectInd]->Pos.x));
        if(Games[App::gameSelectInd]->Pos.x + Games[App::gameSelectInd]->Pos.w >= GameIconArea.x + GameIconArea.w) 
            OffsetGameIcons((GameIconArea.x + GameIconArea.w) - (Games[App::gameSelectInd]->Pos.x + Games[App::gameSelectInd]->Pos.w));
    }
	if(App::dashLayer == 1 && App::appletSelectInd > 0) App::appletSelectInd--;
}

void Dashboard::ActivateDash() {
    if(App::dashLayer == 0) Games[App::gameSelectInd]->Play();
	if(App::dashLayer == 1) Buttons[App::appletSelectInd]->Run();
}

Result Dashboard::OpenSettings() {
	settings->Show();
	return 0;
}

Result Dashboard::CloseSettings() {
	settings->Hide();
	return 0;
}

void Dashboard::UpdateSettings(u32 kDown) {
	settings->Update(kDown);
}