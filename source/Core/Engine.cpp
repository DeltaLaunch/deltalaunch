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

EngineState Engine::State = STATE_DASHBOARD;
MessageBox* MessageBox::instance = nullptr;

Engine::Engine(u32 width, u32 height) {
    //Setup vars
    screenPos.x = screenPos.y = 0;
    screenPos.w = width;
    screenPos.h = height;
    running = true;
    
    //Mount romfs
    romfsMountFromFsdev("/ReiNX/titles/0100000000001000/romfs.bin", 0, "romfs");
    
    //Detect reinx
    if(!Rnx::IsUsingReiNX()) {
        Graphics::Init("", screenPos.w, screenPos.h, "");
        SDL_Surface *surf = IMG_Load("romfs:/Graphics/BSOD.png");
        SDL_Texture *tex = Graphics::CreateTexFromSurf(surf);
        Graphics::RenderTexture(tex, screenPos);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
        Graphics::Render();
        while(!Hid::GetInput());
        Power::Shutdown();
    }
    else
        Rnx::SetHbTidForDelta(0x010000000000100F);
}

Engine::~Engine() {
	delete frndThread;
	delete samsThread;
    delete dash;
    romfsExit();
    Graphics::Exit();
}

void Engine::Initialize() {
    
    //Read config file
    std::string thmPath = Settings::GetCurrentTheme();
    INIReader cfg(thmPath != "" ? (thmPath + ".cfg") : "");
    cfg.SetBasePath(std::string(thmPath+"/"));
    Graphics::Init(TITLE, screenPos.w, screenPos.h, cfg.Get("Config", "font", ""));
    Graphics::SetDefaultSelCol(cfg.GetInteger("Config", "defaultSelCol", 0xFFCEFF));

    //Setup background
    std::vector<std::string> layers;
	std::string tmp;
	int lay;for(lay=0; lay < BACKGROUND_LAYERS; lay++) {
		tmp = cfg.Get("Background", "layer"+ std::to_string(lay), "");
        if(lay == 0 && tmp == "") tmp = "romfs:/Graphics/Wallpaper.png";
		if(tmp == "") continue;
		layers.push_back(tmp);
	}
    
    //Init dashboard
    SDL_Rect batPos; batPos.x = cfg.GetInteger("BatteryOverlay", "x", 1180); batPos.y = cfg.GetInteger("BatteryOverlay", "y", 14);
    SDL_Rect clkPos; clkPos.x = cfg.GetInteger("ClockOverlay", "x", 1110); clkPos.y = cfg.GetInteger("ClockOverlay", "y", 14);
    dash = new Dashboard(screenPos.w, screenPos.h);
    dash->Initialize();
    dash->SetWallpaper(layers);
    dash->SetLockScreen(cfg.Get("Config", "lockscreen_image", "romfs:/Graphics/Lock.png"));
    dash->SetOverlay(cfg.Get("BatteryOverlay", "battery", "romfs:/Graphics/Overlay/Battery.png"), batPos, clkPos);
	dash->settings->SetBackground(cfg.Get("Config", "menus", "romfs:/Graphics/Menu.png"));
    State = (Settings::GetLockScreenFlag() ? STATE_LOCKSCREEN : STATE_DASHBOARD);
	layers.clear();
    
    appletLoadAndApplyIdlePolicySettings();
    appletAllowToEnterSleep();

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
    Hid::Input = Hid::GetInput();
    switch(State) {
        case STATE_LOCKSCREEN:
        {
            Clear();
            dash->DrawLockScreen();
            Render();
            if((Hid::Input & KEY_A) || Hid::IsTouched()) 
                State = STATE_DASHBOARD;
            break;
        }
        case STATE_DASHBOARD:
        {
            //Draw
            dash->DrawWallpaper();
            dash->DrawButtons();
            dash->DrawGames();
            dash->DrawOverlay();
            dash->DrawDebugText();
            if(dash->msgBox->IsOpen()) {
                dash->msgBox->Update();
            }
            else {
                //Get user input
                dash->UpdateDash();
            }
            
            
            //Check state
            if(dash->settings->IsOpen()) State = STATE_SETTINGS;
            else State = STATE_DASHBOARD;
            
            break;
        }
        case STATE_SETTINGS:
        {
            if(!dash->settings->IsOpen())
                State = STATE_DASHBOARD;
            dash->UpdateSettings();
            break;
        }
    }
}