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

#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <functional>
#include <tuple>
#include <string>
#include <switch.h>
#include "Forms.hpp"
#include "Game.hpp"
#include "GameFolder.hpp"
#include "SettingsMenu.hpp"
#include "NewsMenu.hpp"
#include "../Core/Graphics.hpp"
#include "../Core/INI.hpp"
#include "../Services/Hid.hpp"
#include "../Services/Rnx.hpp"
#include "../Services/Settings.hpp"
#include "../Services/Time.hpp"
#include "../Services/Power.hpp"
#include "../Services/App.hpp"
#include "../Services/Hid.hpp"
#include "../Services/Network.hpp"

#define BACKGROUND_LAYERS 99

class Dashboard
{
    public:
        Dashboard(u32 width, u32 height);
        ~Dashboard();
        void Initialize();
        
        //Draw/set
        void DrawWallpaper();
        void SetWallpaper(std::vector<std::string> layers);
        void DrawLockScreen();
        void SetLockScreen(std::string image);
        void DrawButtons();
		void DrawGames();
        void SetGames();
        void DrawOverlay();
        void DrawDebugText();
        
        //Interactions
        void OffsetGameIcons(u32 deltaX);
        SDL_Rect GameIconArea;
		u32 MaxColumns;
		void IncrementDashSel();
		void DecrementDashSel();
        void ActivateDash();
        void UpdateDash();
		
		//Menus
		Result OpenSettings();
		Result CloseSettings();
        Result OpenNews();
		Result CloseNews();
        void UpdateMenus();
        
        bool IsMenuOpen() { return (settings->IsOpen() || news->IsOpen()); }

		u8 gameRows;
		SettingsMenu *settings;
        NewsMenu *news;
        MessageBox *msgBox;
        Network *net;
    private:
        u32 Width, Height;
        u32 lastPosX;
        u32 gamePicSize, gameSpacing;
        SelectType lastMode;
        bool debugInfo;
        bool GcState;
		Result lastErr;
        SDL_Rect BatPos, ClkPos, SetPos;
        SDL_Texture *Wallpaper;
        SDL_Texture *LockScreen;
        SDL_Texture *Battery;
        SDL_Texture *folderIcon;
        std::vector<Button*> Buttons;
		std::vector<GameBase*> GameEntries;
};