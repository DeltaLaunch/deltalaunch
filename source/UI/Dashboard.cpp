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
    gamePicSize = 0, gameSpacing = 0;
    Wallpaper = SDL_CreateTexture(Graphics::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);
    LockScreen = SDL_CreateTexture(Graphics::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);
    msgBox = MessageBox::getInstance();
    net = Network::getInstance();
	SetPos.x=SetPos.y=0; SetPos.w=Width; SetPos.h=Height;
	settings = new SettingsMenu(SetPos);
    settings->Initialize();
    news = new NewsMenu(SetPos);
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
        std::tuple<std::string, std::function<Result()>>{"NewsButton", std::bind(&Dashboard::OpenNews, this)},
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
    
    //News
    SDL_Texture *tex = Graphics::CreateTexFromString("romfs:/Graphics/reinxnews.png");
    news->AddStory("Story 1", "something something news. something something games and shit.1", tex);
    news->AddStory("Story 2", "something something news. something something games and shit.2", tex);
    news->AddStory("Story 3", "something something news. something something games and shit.3", tex);
    
    //Setup overlay
    std::vector<std::string> layers;
	std::string tmp;
	int lay;for(lay=0; lay < BACKGROUND_LAYERS; lay++) {
		tmp = cfg.Get("Background", "layer"+ std::to_string(lay), "");
        if(lay == 0 && tmp == "") tmp = "romfs:/Graphics/Wallpaper.png";
		if(tmp == "") continue;
		layers.push_back(tmp);
	}
    BatPos.x = cfg.GetInteger("BatteryOverlay", "x", 1080); BatPos.y = cfg.GetInteger("BatteryOverlay", "y", 14);
    ClkPos.x = cfg.GetInteger("ClockOverlay", "x", 1150); ClkPos.y = cfg.GetInteger("ClockOverlay", "y", 14);
    SetWallpaper(layers);
    SetLockScreen(cfg.Get("Config", "lockscreen_image", "romfs:/Graphics/Lock.png"));
	settings->SetBackground(cfg.Get("Config", "menus", "romfs:/Graphics/Menu.png"));
    news->SetBackground(cfg.Get("Config", "menus", "romfs:/Graphics/Menu.png"));
    msgBox->SetTexture(cfg.Get("Config", "popups", "romfs:/Graphics/Popup.png"));
    Battery = Graphics::CreateTexFromString(cfg.Get("BatteryOverlay", "battery", "romfs:/Graphics/Overlay/Battery.png"));
    
    layers.clear();
}

/*
*   Draw/Set Graphics
*/
void Dashboard::UpdateDash() {
    //Update as needed
    if(lastMode != Settings::gameSelType) {
        lastMode = Settings::gameSelType;
        SetGames();
    }
    
    if(Hid::Input & KEY_A) ActivateDash();
	if(Hid::Input & KEY_LSTICK)  debugInfo = !debugInfo;
	if(Hid::Input & KEY_MINUS) ;
    if(Hid::Input & KEY_PLUS) msgBox->Show("Test 2", net->Request("http://reinx.guide/api/news?_format=json", Network::HTTP_GET), MSGBOX_YESNO);
	if((Hid::Input & KEY_DLEFT) || (Hid::Input & KEY_LSTICK_LEFT)) DecrementDashSel();
	if((Hid::Input & KEY_DRIGHT) || (Hid::Input & KEY_LSTICK_RIGHT)) IncrementDashSel();
	if((Hid::Input & KEY_DUP) || (Hid::Input & KEY_LSTICK_UP)) App::dashLayer = 0;
	if((Hid::Input & KEY_DDOWN) || (Hid::Input & KEY_LSTICK_DOWN)) App::dashLayer = 1;
	if(Hid::IsMoving(GameIconArea)) {
		if(lastPosX != 0) 
			OffsetGameIcons(Hid::GetTouchPos().px - lastPosX);
		lastPosX = Hid::GetTouchPos().px;
	} else {
		lastPosX = 0;
	}
    
    //Check button interactions
    for(auto button: Buttons) {
        //Detect touch selection
        if((Hid::IsTouched(button->Pos) && !Hid::IsMoving()) && !IsMenuOpen()) {
            lastErr = button->Run();
			if(lastErr) 
                App::ShowError("An Error has occurred!", "Error code: " + std::to_string(lastErr), lastErr);
            appletRequestForeground();
		}
    }
    
    //Check game interactions
    u32 ind = 0;
    for(auto entry: GameEntries) {
        //Detect touch selection
        if(!IsMenuOpen() && Hid::IsTouched(entry->Pos)) {
            if (ind == App::gameSelectInd && !Hid::IsMoving()) {
                //Game
                if(entry->GetTitleId() && entry->FolderID == 0) {
                    lastErr = entry->Run();
                    if(lastErr) 
                        App::ShowError("An Error has occurred!", "Error code: " + std::to_string(lastErr), lastErr);
                    appletRequestForeground();
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
    Graphics::DrawText(Fonts::FONT_MEDIUM, 25, 570, Time::GetDate());
    Graphics::DrawText(Fonts::FONT_HUGE, 25, 600, Time::GetClock(Time::CLOCK_12HR));
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
        switch(Settings::gameSelType) {
            //Classic outline format
            default:
            case SELECT_OUTLINE:
            {
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
                break;
            }
            //Diff sizes; selected is highlighted
            case SELECT_SIZEDIFF:
            {
                //Draw game bigger
                if(ind == App::gameSelectInd && App::dashLayer == 0) {
                    SDL_Rect pos = entry->Pos; 
                    pos.x -= 20; pos.y -= 20; pos.w += 40; pos.h += 40;
                    Graphics::RenderTexture(entry->Icon, pos);
                }
                else {
                    //Draw either game icon or backer
                    if(entry->GetTitleId() != 0) {
                        SDL_Rect pos = entry->Pos;
                        if(ind > App::gameSelectInd && App::dashLayer == 0) pos.x += 20;
                        if(ind < App::gameSelectInd && App::dashLayer == 0) pos.x -= 20;
                        Graphics::RenderTexture(entry->Icon, entry->Pos);
                    }
                    Graphics::Rectangle(entry->Pos, 0x70);
                }
                break;
            }
            //PS4 style
            case SELECT_PSSTYLE:
            {
                //Draw selection outline
                if(ind == App::gameSelectInd && App::dashLayer == 0) {
                    SDL_Rect pos = entry->Pos; 
                    pos.h = ((entry->Pos.h * 2) + 40);
                    pos.w = (entry->Pos.w * 2);
                    Graphics::Rectangle(pos, 0x666666FF);
                    pos.h = (entry->Pos.h * 2);
                    Graphics::RenderTexture(entry->Icon, pos);
                    Graphics::DrawText(Fonts::FONT_SMALL, pos.x + 105, pos.y + pos.h + 8, "Start", 0xFFFFFFFF);
                    if(gameRows == 1) Graphics::DrawText(Fonts::FONT_SMALL, pos.x + pos.w + 20, pos.y + pos.h + 8, entry->GetName(), Graphics::GetDefaultSelCol());
                }
                else {
                    SDL_Rect pos = entry->Pos; 
                    //Draw either game icon or backer
                    if(ind >= App::gameSelectInd && App::dashLayer == 0) pos.x += 128;
                    if(entry->Icon != nullptr) {
                        Graphics::RenderTexture(entry->Icon, pos);
                    } 
                    else {
                        Graphics::Rectangle(pos, 0x70);
                    }
                }
                break;
            }
        }
		ind++;
	}
}

void Dashboard::SetGames() {
    
    switch(Settings::gameSelType) {
        case SELECT_OUTLINE: { gamePicSize = 256; gameSpacing = 14; break; }
        case SELECT_SIZEDIFF: { gamePicSize = 256; gameSpacing = 0; break; }
        case SELECT_PSSTYLE: { gamePicSize = 128; gameSpacing = 0; break; }
    }
    
    u32 normalPortraitSize = gamePicSize;
    GameIconArea.x = 120; GameIconArea.y = 110;
    GameIconArea.w = 1080; GameIconArea.h = normalPortraitSize;
    
    //Create game images
	std::vector<NsApplicationRecord> recs;
    App::GetAppRecords(recs);
    int i = 0;
	size_t total = recs.size();
	for(auto game: GameEntries) {
		u64 tid = i < (int)total ? recs[i].titleID : 0;
        game->Pos.w = normalPortraitSize/gameRows; 
        game->Pos.h = normalPortraitSize/gameRows;
        game->Pos.x = /*(1280/2)-((game->Pos.w*(MaxColumns/gameRows))/2)*/100+((i%(MaxColumns/gameRows))*(game->Pos.w+gameSpacing)); 
        game->Pos.y = 200+((i/(MaxColumns/gameRows))*(game->Pos.h+gameSpacing));
        SDL_Surface *img;
        if(tid != game->GetTitleId()) { //assume game entry doesnt need to be updated if tids are the same
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
    BatPos.w = 53*6; BatPos.h = 25;
    SDL_Rect spritePos = BatPos;
    spritePos.w = BatPos.w/6;
    spritePos.h = spritePos.h;
    spritePos.x = spritePos.w * (Power::GetBatteryLife()/20);
    spritePos.y = 0;
    
	Graphics::RenderTexture(Battery, BatPos, &spritePos);
    Graphics::DrawText(Fonts::FONT_SMALL, ClkPos.x, ClkPos.y, Time::GetClock(Time::CLOCK_12HR));
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

Result Dashboard::OpenNews() {
	news->Show();
	return 0;
}

Result Dashboard::CloseNews() {
	news->Hide();
	return 0;
}

void Dashboard::UpdateMenus() {
	news->Update();
    settings->Update();
}