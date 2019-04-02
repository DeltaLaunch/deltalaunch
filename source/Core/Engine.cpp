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

    Graphics::Init(TITLE, width, height);
    HeapAddr = heapAddr;
    HeapSize = heapSize;
    Width = width;
    Height = height;
}

Engine::~Engine() {
	delete frndThread;
	delete samsThread;
    delete dash;
    Graphics::Exit();
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
    dash = new Dashboard(Width, Height, (baseThemeDir+cfg.Get("Config", "font", "")).c_str());
    dash->SetWallpaper(layers);
    dash->SetLockScreen(baseThemeDir + cfg.Get("Config", "lockscreen_image", ""));
    dash->SetOverlay(baseThemeDir + cfg.Get("BatteryOverlay", "battery", ""), batPos, clkPos);
	dash->settings->SetBackground(baseThemeDir + cfg.Get("Menus", "settings", ""));
    State = (Settings::GetLockScreenFlag() ? STATE_LOCKSCREEN : STATE_DASHBOARD);
	layers.clear();
    
    //Create buttons to add to dash
    unsigned x = 230;       //padding on edges
    unsigned space = 100;   //space inbetween
    std::vector<std::tuple<std::string, std::function<Result()>>> Buttons{
        std::tuple<std::string, std::function<Result()>>{"WebButton", std::bind(App::LaunchWebsite, "https://google.com/")},
        std::tuple<std::string, std::function<Result()>>{"NewsButton", nullptr},
        std::tuple<std::string, std::function<Result()>>{"ShopButton", App::LaunchShop},
        std::tuple<std::string, std::function<Result()>>{"AlbumButton", std::bind(App::LaunchAlbum, 1, false)}, //1,false | 2,true
        std::tuple<std::string, std::function<Result()>>{"HomebrewButton", App::LaunchHbl},
        std::tuple<std::string, std::function<Result()>>{"SettingsButton", std::bind(&Dashboard::OpenSettings, dash)},
        std::tuple<std::string, std::function<Result()>>{"PowerButton", Power::Reboot},
    };
    for(int but = 0; but < (int)Buttons.size(); but++) {
        dash->AddButton(
            new Button(
                baseThemeDir + cfg.Get(std::get<0>(Buttons.at(but)), "sprite", ""), 
                baseThemeDir + cfg.Get(std::get<0>(Buttons.at(but)), "sprite_select", ""), 
                cfg.GetInteger(std::get<0>(Buttons.at(but)), "x", x+=space), 
                cfg.GetInteger(std::get<0>(Buttons.at(but)), "y", 600), 
                std::get<1>(Buttons.at(but))
            )
        );
    }
    
    //Boundries: (120, 110), (1200, 560) .. 450px vert
    for(int i = 0; i < (int)dash->MaxColumns; i++){
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
    Graphics::Render();
}

void Engine::Clear() {
    Graphics::Clear();
}

void Engine::Update() {
	//Get hid input
    hidScanInput();
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    
    //Lockscreen
    if(State == STATE_LOCKSCREEN) {
        Clear();
        dash->DrawLockScreen();
        Render();
        if((kDown & KEY_A) || Hid::IsTouched()) 
            State = STATE_DASHBOARD;
    }
    else {
        State = dash->IsMenuOpen ? STATE_SETTINGS : STATE_DASHBOARD;
    }
    
    //Dash
	if(State == STATE_DASHBOARD) {
        dash->UpdateDash(kDown);
		dash->DrawWallpaper();
		dash->DrawButtons();
		dash->DrawGames();
		dash->DrawOverlay();
		dash->DrawDebugText();
	}
	if(State == STATE_SETTINGS)
		dash->UpdateSettings(kDown);
}