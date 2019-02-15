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
        fatalSimple(0xDEADBABE);
    }
    
    //Read config file
    baseThemeDir = "/Theme/";
    INIReader cfg(baseThemeDir + "theme.cfg");
    HeapAddr = heapAddr;
    HeapSize = heapSize;
    Width = width;
    Height = height;
    
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
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    Mix_VolumeMusic(cfg.GetInteger("Background", "bgmVol", 64));
    
    //Setup background
    bgm = Mix_LoadMUS(cfg.Get("Background", "bgm", "").c_str());
    std::string bgLay0 = cfg.Get("Background", "layer0", "");
    std::string bgLay1 = cfg.Get("Background", "layer1", "");
    std::string bgLay2 = cfg.Get("Background", "layer2", "");
    
    //Init dashboard
    dash = new Dashboard(&mRender, Width, Height, (baseThemeDir+cfg.Get("Config", "font", "")).c_str());
    dash->SetWallpaper(baseThemeDir + bgLay0, baseThemeDir + bgLay1, baseThemeDir + bgLay2);
    dash->SetLockScreen(baseThemeDir + cfg.Get("Config", "lockscreen_image", ""));
    screenLocked = cfg.GetBoolean("Config", "lockscreen_enable", true);
    
    //Create buttons to add to dash
    unsigned x = 230;       //padding on edges
    unsigned space = 100;   //space inbetween
    dash->AddButton(new Button(baseThemeDir + cfg.Get("WebButton", "sprite", ""), cfg.GetInteger("WebButton", "x", x+=space), cfg.GetInteger("WebButton", "y", 600), &mRender, std::bind(App::LaunchWebsite, "https://google.com/")));
    dash->AddButton(new Button(baseThemeDir + cfg.Get("NewsButton", "sprite", ""), cfg.GetInteger("NewsButton", "x", x+=space), cfg.GetInteger("NewsButton", "y", 600), &mRender, nullptr));
    dash->AddButton(new Button(baseThemeDir + cfg.Get("ShopButton", "sprite", ""), cfg.GetInteger("ShopButton", "x", x+=space), cfg.GetInteger("ShopButton", "y", 600), &mRender, App::LaunchShop));
    dash->AddButton(new Button(baseThemeDir + cfg.Get("AlbumButton", "sprite", ""), cfg.GetInteger("AlbumButton", "x", x+=space), cfg.GetInteger("AlbumButton", "y", 600), &mRender, App::LaunchAlbum));
    dash->AddButton(new Button(baseThemeDir + cfg.Get("HomebrewButton", "sprite", ""), cfg.GetInteger("HomebrewButton", "x", x+=space), cfg.GetInteger("HomebrewButton", "y", 600), &mRender, App::LaunchHbl));
    dash->AddButton(new Button(baseThemeDir + cfg.Get("SettingsButton", "sprite", ""), cfg.GetInteger("SettingsButton", "x", x+=space), cfg.GetInteger("SettingsButton", "y", 600), &mRender, std::bind(&Dashboard::OpenMenu, dash, "Settings")));
    dash->AddButton(new Button(baseThemeDir + cfg.Get("PowerButton", "sprite", ""), cfg.GetInteger("PowerButton", "x", x+=space), cfg.GetInteger("PowerButton", "y", 600), &mRender, Power::Shutdown));
	
	dash->AddMenu(new Menu("Settings", "", 0, 0, baseThemeDir + cfg.Get("Menus", "settings", ""), &mRender));
    
    //Create game images
    //App::GetList();
    //Boundries: (120, 110), (x, 560) .. 450px vert
    int i, colums = 4, rows = 1;
    for(i = 0; i < colums*rows; i++)
        dash->AddButton(new Button(100+(i*270), 110+(225-(rows*135))+((i%rows)*270), 256, 256, 0x70, nullptr));
    
    //Play BGM
    if(bgm) Mix_PlayMusic(bgm, -1);
}

Engine::~Engine() {
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

void Engine::Render() {
    SDL_RenderPresent(mRender._renderer);
}

void Engine::Clear() {
    SDL_RenderClear(mRender._renderer);
}

void Engine::GetInputs() {
    hidScanInput();
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    
    if(kDown & KEY_A) {
        if(screenLocked) screenLocked = false;
    }

	if(kDown & KEY_B) {
		if(dash->IsMenuOpen) {
			dash->CloseMenus();
            dash->IsMenuOpen = false;
		}
	}
    
    if (kDown & KEY_PLUS) {
        dash->ToggleDebug();
    }
    
    if ((kDown & KEY_DUP) || (kDown & KEY_LSTICK_UP)) {
        
    }
    
    if ((kDown & KEY_DDOWN) || (kDown & KEY_LSTICK_DOWN)) {
        
    }
}

void Engine::Update() {
    
    // 0) Lockscreen
    while(screenLocked && !Hid::IsTouched()) {
        Clear();
        dash->DrawLockScreen();
        Render();
        GetInputs();
    }
    screenLocked = false;
    GetInputs();
    
    // 1) Draw wallpaper
    dash->DrawWallpaper();
    
    // 2) Draw overlay
    dash->DrawButtons();
    
    // 3) Draw menus
    dash->DrawMenus();
    
    // 3) Draw debug text
    dash->DrawDebugText();
}