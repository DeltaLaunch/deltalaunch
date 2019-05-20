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
    msgBox = MessageBox::getInstance();
	SetPos.x=SetPos.y=0; SetPos.w=Width; SetPos.h=Height;
	settings = new SettingsMenu(SetPos);
    settings->Initialize();
}

Dashboard::~Dashboard() {
	delete settings;
	for(auto button: Buttons) delete button;
	for(auto game: GameEntries) delete game;
    Buttons.clear();
	GameEntries.clear();
    SDL_DestroyTexture(Wallpaper);
    SDL_DestroyTexture(LockScreen);
	SDL_DestroyTexture(Battery);
}

void Dashboard::Initialize() {
    //Create buttons to add to dash
    unsigned x = 230;       //padding on edges
    unsigned space = 100;   //space inbetween
    std::string thmPath = Settings::GetCurrentTheme();
    INIReader cfg(thmPath != "" ? (thmPath + ".cfg") : "");
    cfg.SetBasePath(std::string(thmPath+"/"));
    std::vector<std::tuple<std::string, std::function<Result()>>> ButtonFuncs{
        std::tuple<std::string, std::function<Result()>>{"WebButton", std::bind(App::LaunchWebsite, "https://google.com/")},
        std::tuple<std::string, std::function<Result()>>{"NewsButton", App::LaunchNews},
        std::tuple<std::string, std::function<Result()>>{"ShopButton", App::LaunchShop},
        std::tuple<std::string, std::function<Result()>>{"AlbumButton", std::bind(App::LaunchAlbum, 2, true)}, //1,false | 2,true
        std::tuple<std::string, std::function<Result()>>{"HomebrewButton", App::LaunchHbl},
        std::tuple<std::string, std::function<Result()>>{"SettingsButton", std::bind(&Dashboard::OpenSettings, this)},
        std::tuple<std::string, std::function<Result()>>{"SleepButton", Power::Reboot},
    };
    for(u32 but = 0; but < ButtonFuncs.size(); but++) {
        Buttons.push_back(new Button(
            cfg.Get(std::get<0>(ButtonFuncs.at(but)), "sprite", "romfs:/Graphics/Icons/" + std::get<0>(ButtonFuncs.at(but)) + ".png"), 
            cfg.Get(std::get<0>(ButtonFuncs.at(but)), "sprite_select", "romfs:/Graphics/Icons/" + std::get<0>(ButtonFuncs.at(but)) + "_select.png"), 
            cfg.GetInteger(std::get<0>(ButtonFuncs.at(but)), "x", x+=space), 
            cfg.GetInteger(std::get<0>(ButtonFuncs.at(but)), "y", 600), 
            std::get<1>(ButtonFuncs.at(but))
        ));
    }
    
    //Setup folder sprite
    SDL_Surface *img = IMG_Load(cfg.Get("Folders", "icon", "romfs:/Graphics/Folder.png").c_str());
    if(img) folderIcon = Graphics::CreateTexFromSurf(img);
    
    std::vector<NsApplicationRecord> recs;
    App::GetAppRecords(recs);
    size_t recCnt = recs.size();
    recs.clear();
    u32 fold = 0;
    //Boundries: (120, 110), (1200, 560) .. 450px vert
    for(u32 i = 0; i < MaxColumns; i++){
		if(i <= recCnt-1)
            GameEntries.push_back(new Game());
        else 
            GameEntries.push_back(new GameFolder(folderIcon, fold++));
	}
    SetGames();
}

/*
*   Draw/Set Graphics
*/
void Dashboard::UpdateDash() { 
    if(Hid::Input & KEY_A) ActivateDash();
	if(Hid::Input & KEY_LSTICK)  debugInfo = !debugInfo;
	if(Hid::Input & KEY_MINUS) msgBox->Show("Test", "hello", MSGBOX_OK);
    if(Hid::Input & KEY_PLUS) msgBox->Show("Test 2", "testing....", MSGBOX_YESNO);
	if((Hid::Input & KEY_DLEFT) || (Hid::Input & KEY_LSTICK_LEFT)) DecrementDashSel();
	if((Hid::Input & KEY_DRIGHT) || (Hid::Input & KEY_LSTICK_RIGHT)) IncrementDashSel();
	if((Hid::Input & KEY_DUP) || (Hid::Input & KEY_LSTICK_UP)) App::dashLayer = 0;
	if((Hid::Input & KEY_DDOWN) || (Hid::Input & KEY_LSTICK_DOWN)) App::dashLayer = 1;
	if(Hid::IsTouched(GameIconArea)) {
		if(lastPosX != 0) 
			OffsetGameIcons(Hid::GetTouchPos().px - lastPosX);
		lastPosX = Hid::GetTouchPos().px;
	} else {
		lastPosX = 0;
	}
    
    //Check button interactions
    for(auto button: Buttons) {
        //Detect touch selection
        if(Hid::IsTouched(button->Pos) && !settings->IsOpen()) {
            lastErr = button->Run();
			if(lastErr) App::ShowError("An Error has occurred!", "Error code: " + std::to_string(lastErr), lastErr);
		}
    }
    
    //Check game interactions
    u32 ind = 0;
    for(auto entry: GameEntries) {
        //Detect touch selection
        if(!settings->IsOpen() && Hid::IsTouched(entry->Pos)) {
            if (ind == App::gameSelectInd) {
                //Game
                if(entry->GetTitleId() && entry->FolderID == 0) {
                    lastErr = entry->Run();
                    if(lastErr) 
                        App::ShowError("An Error has occurred!", "Error code: " + std::to_string(lastErr), lastErr);
                }
                //Folder
                else if(entry->FolderID != 0) {
                    //TODO
                }
            } else {
                App::gameSelectInd = ind;
            }
		}
        ind++;
    }
    
    /*if(App::IsGamecardInserted() == GcState) {
        SetGames();
        GcState = !GcState;
    }*/
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
		ind++;
    }
}

void Dashboard::DrawGames() {
	unsigned ind = 0;
	for(auto entry: GameEntries) {
        //Classic outline format
        if(Settings::gameSelType == SELECT_OUTLINE) {
            //Draw selection outline
            if(ind == App::gameSelectInd && App::dashLayer == 0) {
                SDL_Rect pos = entry->Pos; 
                pos.x -= 5; pos.y -= 5; pos.w += 10; pos.h += 10;
                Graphics::Rectangle(pos, Graphics::GetDefaultSelCol());
                if(gameRows == 1) Graphics::DrawText(Fonts::FONT_SMALL, pos.x, pos.y-25, entry->GetName(), Graphics::GetDefaultSelCol());
            }
            
            //Draw either game icon or backer
            if(entry->Icon != nullptr) {
                Graphics::RenderTexture(entry->Icon, entry->Pos);
            } 
            else {
                Graphics::Rectangle(entry->Pos, 0x70);
            }
        }
		//Or draw size diff mode
		if(Settings::gameSelType == SELECT_SIZEDIFF) {
            //Draw game bigger
            if(ind == App::gameSelectInd && App::dashLayer == 0) {
                SDL_Rect pos = entry->Pos; 
                pos.x -= 20; pos.y -= 20; pos.w += 40; pos.h += 40;
                Graphics::RenderTexture(entry->Icon, pos);
            }
            else {
                //Draw either game icon or backer
                if(entry->GetTitleId() != 0) {
                    Graphics::RenderTexture(entry->Icon, entry->Pos);
                }
                Graphics::Rectangle(entry->Pos, 0x70);
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
	std::vector<NsApplicationRecord> recs;
    App::GetAppRecords(recs);
    int i = 0;
	size_t total = recs.size();
	for(auto game: GameEntries) {
		u64 tid = i < (int)total ? recs[i].titleID : 0;
        game->Pos.w = normalPortraitSize/gameRows; 
        game->Pos.h = normalPortraitSize/gameRows;
        game->Pos.x = /*(1280/2)-((game->Pos.w*(MaxColumns/gameRows))/2)*/100+((i%(MaxColumns/gameRows))*(game->Pos.w+(14/gameRows))); 
        game->Pos.y = 200+((i/(MaxColumns/gameRows))*(game->Pos.h+(14/gameRows)));
        SDL_Surface *img;
        if(tid != 0) { //assume game entry doesnt need to be updated if tids are the same
            game->SetTitleId(tid);
            NsApplicationControlData data = App::GetGameControlData(tid, 0);
            img = IMG_Load_RW(SDL_RWFromMem(data.icon, 0x20000), 1);
            if(game->Icon != nullptr)
                SDL_DestroyTexture(game->Icon);
            if(img) {
                game->Icon = Graphics::CreateTexFromSurf(img);
                SDL_FreeSurface(img);
            }
            int i;
            for(i = 0; i < 15; i++){
                if(data.nacp.lang[i].name[0]==0) continue;
                game->SetName(data.nacp.lang[i].name);
            }
            for(i = 0; i < 15; i++){
                if(data.nacp.lang[i].author[0]==0) continue;
                game->SetAuthor(data.nacp.lang[i].author);
            }
        }
        i++;
	}
    recs.clear();
}

void Dashboard::DrawOverlay() {
	Graphics::RenderTexture(Battery, BatPos);
    Graphics::DrawText(Fonts::FONT_SMALL, ClkPos.x, ClkPos.y, Time::GetClock());
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
        Graphics::DrawText(Fonts::FONT_TINY, X, Y+=s, "DeltaLaunch alpha!");
        Graphics::DrawText(Fonts::FONT_TINY, X, Y+=s, "Firmware: " + Settings::GetFirmwareVersion());
        Graphics::DrawText(Fonts::FONT_TINY, X, Y+=s, "Serial: " + Settings::GetSerialNumber());
		Graphics::DrawText(Fonts::FONT_TINY, X, Y+=s, "Battery: " + std::to_string(Power::GetBatteryLife()) + "%");
        Graphics::DrawText(Fonts::FONT_TINY, X, Y+=s, "Touch: X=" + std::to_string(touchPos.px) + "; y=" + std::to_string(touchPos.py));
        Graphics::DrawText(Fonts::FONT_TINY, X, Y+=s, "Mem avail: " + std::to_string(Settings::GetMemAvail()));
        Graphics::DrawText(Fonts::FONT_TINY, X, Y+=s, "Mem used: " + std::to_string(Settings::GetMemUsed()));
        Graphics::DrawText(Fonts::FONT_TINY, X, Y+=s, "Ae Message: " + std::to_string(App::lastAeCmd));
		Graphics::DrawText(Fonts::FONT_TINY, X, Y+=s, "Sams Message: " + std::to_string(App::lastSamsCmd));
        Graphics::DrawText(Fonts::FONT_TINY, X, Y+=s, "Last Error: " + std::to_string(lastErr));
        Y = 0;
    }
}

/*
*   Trigger events
*/
void Dashboard::OffsetGameIcons(u32 deltaX) {
    int i = 0;
    for(auto game: GameEntries) {
		game->Pos.x = (game->Pos.x <= 100+(i*(game->Pos.w+(14/gameRows)))) ? (game->Pos.x + deltaX) : 100+(i*(game->Pos.w+(14/gameRows)));
        i++;
	}
}

void Dashboard::IncrementDashSel() {
	if(App::dashLayer == 0 && App::gameSelectInd < GameEntries.size()-1) {
        App::gameSelectInd++;
        if(GameEntries[App::gameSelectInd]->Pos.x < 0) 
            OffsetGameIcons(100 - (GameEntries[App::gameSelectInd]->Pos.x));
        if(GameEntries[App::gameSelectInd]->Pos.x + GameEntries[App::gameSelectInd]->Pos.w >= GameIconArea.x + GameIconArea.w) 
            OffsetGameIcons((GameIconArea.x + GameIconArea.w) - (GameEntries[App::gameSelectInd]->Pos.x + GameEntries[App::gameSelectInd]->Pos.w));
    }
	if(App::dashLayer == 1 && App::appletSelectInd < Buttons.size()-1) App::appletSelectInd++;
}

void Dashboard::DecrementDashSel() {
	if(App::dashLayer == 0 && App::gameSelectInd > 0) {
        App::gameSelectInd--;
        if(GameEntries[App::gameSelectInd]->Pos.x < 0) 
            OffsetGameIcons(100 - (GameEntries[App::gameSelectInd]->Pos.x));
        if(GameEntries[App::gameSelectInd]->Pos.x + GameEntries[App::gameSelectInd]->Pos.w >= GameIconArea.x + GameIconArea.w) 
            OffsetGameIcons((GameIconArea.x + GameIconArea.w) - (GameEntries[App::gameSelectInd]->Pos.x + GameEntries[App::gameSelectInd]->Pos.w));
    }
	if(App::dashLayer == 1 && App::appletSelectInd > 0) App::appletSelectInd--;
}

void Dashboard::ActivateDash() {
    if(App::dashLayer == 0 && (GameEntries[App::gameSelectInd]->GetTitleId() != 0 || GameEntries[App::gameSelectInd]->FolderID != 0)) {
        GameEntries[App::gameSelectInd]->Run();
        SetGames();
    }
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

void Dashboard::UpdateSettings() {
	settings->Update();
}