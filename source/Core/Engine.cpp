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

#include "Engine.hpp"

Engine::Engine(u32 width, u32 height, void *heapAddr, size_t heapSize) {
    //Detect reinx
    if(!Rnx::IsUsingReiNX()) {
        fatalSimple(0xBADC0DE);
    }
    
    //Basic SDL init
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(width, height, 0, &mRender._window, &mRender._renderer);
    mRender._surface = SDL_GetWindowSurface(mRender._window);
    SDL_SetRenderDrawBlendMode(mRender._renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    SDL_SetRenderDrawColor(mRender._renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
    HeapAddr = heapAddr;
    HeapSize = heapSize;
    Width = width;
    Height = height;
}

Engine::~Engine() {
	delete frndThread;
	delete samsThread;
    delete dash;
    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_DestroyRenderer(mRender._renderer);
    SDL_FreeSurface(mRender._surface);
    SDL_DestroyWindow(mRender._window);
    SDL_Quit();
}

void Engine::Initialize() {
    //Read config file
    baseThemeDir = "/Theme/";
    INIReader cfg(baseThemeDir + "theme.cfg");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    Mix_VolumeMusic(cfg.GetInteger("Background", "bgmVol", 64));

    //Setup background
    bgm = Mix_LoadMUS(cfg.Get("Background", "bgm", "").c_str());
    std::vector<std::string> layers;
	std::string tmp;
	int lay;for(lay=0; lay < BACKGROUND_LAYERS; lay++) {
		tmp = cfg.Get("Background", "layer"+ std::to_string(lay), "");
		if(tmp == "") continue;
		layers.push_back(baseThemeDir + tmp);
	}
    
    //Init dashboard
    SDL_Rect batPos; batPos.x = cfg.GetInteger("BatteryOverlay", "x", 1180); batPos.y = cfg.GetInteger("BatteryOverlay", "y", 14);
    SDL_Rect clkPos; clkPos.x = cfg.GetInteger("ClockOverlay", "x", 1110); clkPos.y = cfg.GetInteger("ClockOverlay", "y", 14);
    dash = new Dashboard(&mRender, Width, Height, (baseThemeDir+cfg.Get("Config", "font", "")).c_str());
    dash->SetWallpaper(layers);
    dash->SetLockScreen(baseThemeDir + cfg.Get("Config", "lockscreen_image", ""));
    dash->SetOverlay(baseThemeDir + cfg.Get("BatteryOverlay", "battery", ""), batPos, clkPos);
    State = (Settings::GetLockScreenFlag() ? STATE_LOCKSCREEN : STATE_DASHBOARD);
	layers.clear();
    
    //Create buttons to add to dash
    unsigned x = 230;       //padding on edges
    unsigned space = 100;   //space inbetween
    std::vector<std::tuple<std::string, std::function<Result()>>> Buttons{
        std::tuple<std::string, std::function<Result()>>{"WebButton", std::bind(App::LaunchWebsite, "https://google.com/")},
        std::tuple<std::string, std::function<Result()>>{"NewsButton", nullptr},
        std::tuple<std::string, std::function<Result()>>{"ShopButton", App::LaunchShop},
        std::tuple<std::string, std::function<Result()>>{"AlbumButton", App::LaunchAlbum},
        std::tuple<std::string, std::function<Result()>>{"HomebrewButton", App::LaunchHbl},
        std::tuple<std::string, std::function<Result()>>{"SettingsButton", std::bind(&Dashboard::OpenMenu, dash, "Settings")},
        std::tuple<std::string, std::function<Result()>>{"PowerButton", Power::Reboot},
    };
    for(int but = 0; but < Buttons.size(); but++) {
        dash->AddButton(
            new Button(
                baseThemeDir + cfg.Get(std::get<0>(Buttons.at(but)), "sprite", ""), 
                baseThemeDir + cfg.Get(std::get<0>(Buttons.at(but)), "sprite_select", ""), 
                cfg.GetInteger(std::get<0>(Buttons.at(but)), "x", x+=space), 
                cfg.GetInteger(std::get<0>(Buttons.at(but)), "y", 600), 
                &mRender, std::get<1>(Buttons.at(but))
            )
        );
    }
	
    //Settings
    Menu *settings = new Menu("Settings", "", 0, 0, baseThemeDir + cfg.Get("Menus", "settings", ""), &mRender);
    u32 Y = 20, butW = 200, butH = 60, butCol = 0x202020FF;
    space = 20+butH;
    settings->AddButton(new Button("Lock Screen", 60, Y+=space, butW, butH, butCol, nullptr));
    settings->AddButton(new Button("Internet", 60, Y+=space, butW, butH, butCol, nullptr));
    settings->AddButton(new Button("Profile", 60, Y+=space, butW, butH, butCol, nullptr));
    settings->AddButton(new Button("Select Mode", 60, Y+=space, butW, butH, butCol, nullptr));
    settings->AddButton(new Button("System Info", 60, Y+=space, butW, butH, butCol, nullptr));
	dash->AddMenu(settings);
    
    //Boundries: (120, 110), (1200, 560) .. 450px vert
    for(int i = 0; i < dash->MaxColumns; i++){
		dash->AddGame(new Game());
	}
    dash->SetGames();
    
    //Play BGM
    if(bgm) Mix_PlayMusic(bgm, -1);

	//Start threads
    frndThread = new ThreadManager(Threads::FriendThread);
    frndThread->start();
	
	samsThread = new ThreadManager(Threads::SystemAppletMessage);
	samsThread->start();
	
	aemThread = new ThreadManager(Threads::AeMessageThread);
	aemThread->start();
}

void Engine::Render() {
    SDL_RenderPresent(mRender._renderer);
}

void Engine::Clear() {
    SDL_RenderClear(mRender._renderer);
}

void Engine::GetInputs() {
    //Get hid input
    hidScanInput();
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    
    switch(State) {
        case STATE_LOCKSCREEN:
        {
            if(kDown & KEY_A) State = STATE_DASHBOARD;
            break;
        }
        case STATE_DASHBOARD:
        {
            if(kDown & KEY_A) dash->ActivateDash();
            if(kDown & KEY_PLUS) dash->ToggleDebug();
            if(kDown & KEY_MINUS) ;
			if(kDown & KEY_DLEFT) dash->DecrementDashSel();
			if(kDown & KEY_DRIGHT) dash->IncrementDashSel();
			if(kDown & KEY_DUP) dash->selLayer = 0;
			if(kDown & KEY_DDOWN) dash->selLayer = 1;
            if(Hid::IsTouched(dash->GameIconArea) && !Hid::IsTapped(dash->GameIconArea)) {
                if(lastPosX != 0) 
                    dash->OffsetGameIcons(Hid::GetTouchPos().px - lastPosX);
                lastPosX = Hid::GetTouchPos().px;
            } else {
                lastPosX = 0;
            }
            break;
        }
        case STATE_SETTINGS:
        {
            if(kDown & KEY_A) dash->ActivateMenu();
			if(kDown & KEY_B) dash->DisengageMenu();
			if(kDown & KEY_DUP) dash->DecrementMenuSel();
			if(kDown & KEY_DDOWN) dash->IncrementMenuSel();
            break;
        }
    }

    if(App::IsGamecardInserted() == GcState) {
        dash->SetGames();
        GcState = !GcState;
    }
}

void Engine::Update() {
    
    //Lockscreen
    while(State == STATE_LOCKSCREEN && !Hid::IsTouched()) {
        Clear();
        dash->DrawLockScreen();
        Render();
        GetInputs();
    }
    State = dash->IsMenuOpen ? STATE_SETTINGS : STATE_DASHBOARD;
    GetInputs();
    
    //Dash
    dash->DrawWallpaper();
    dash->DrawButtons();
	dash->DrawGames();
    dash->DrawOverlay();
    dash->DrawMenus();
    dash->DrawDebugText();
}