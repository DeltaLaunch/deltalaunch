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
    screenPos.x = 0;
    screenPos.y = 0;
    screenPos.w = width;
    screenPos.h = height;
    running = true;
    
    //Detect reinx
    if(!Rnx::IsUsingReiNX()) {
        if(R_SUCCEEDED(romfsMountFromFsdev("/atmosphere/titles/0100000000001000/romfs.bin", 0, "romfs"))) goto err;
        if(R_SUCCEEDED(romfsMountFromFsdev("/sxos/titles/0100000000001000/romfs.bin", 0, "romfs"))) goto err;
        err:
        Graphics::Init(screenPos);
        SDL_Surface *surf = IMG_Load("romfs:/Graphics/BSOD.png");
        SDL_Texture *tex = Graphics::CreateTexFromSurf(surf);
        Graphics::RenderTexture(tex, screenPos);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
        Graphics::Render();
        do { Hid::KeyProcess(); } while(!Hid::Input);
        Power::Shutdown();
    }
    
    //Mount romfs
    romfsMountFromFsdev("/ReiNX/titles/0100000000001000/romfs.bin", 0, "romfs");
    
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
    Graphics::Init(screenPos);
    Graphics::SetDefaultSelCol(cfg.GetInteger("Config", "defaultSelCol", 0xFFCEFF));
    Hid::TouchInit();

    //Init dashboard
    dash = new Dashboard(screenPos.w, screenPos.h);
    dash->Initialize();
    
    State = (Settings::GetLockScreenFlag() ? STATE_LOCKSCREEN : STATE_DASHBOARD);
    
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
    Hid::KeyProcess();
    Hid::TouchProcess();
    
    switch(State) {
        case STATE_LOCKSCREEN:
        {
            dash->DrawLockScreen();
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
            dash->UpdateDash();
            
            //Check state
            if(dash->IsMenuOpen()) 
                State = STATE_MENU;
            else 
                State = STATE_DASHBOARD;
            
            break;
        }
        case STATE_MENU:
        {
            dash->UpdateMenus();
            if(!dash->IsMenuOpen())
                State = STATE_DASHBOARD;
            break;
        }
    }
}