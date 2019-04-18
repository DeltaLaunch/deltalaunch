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
#include <SDL2/SDL2_gfxPrimitives.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <functional>
#include <tuple>
#include <string>
#include <switch.h>
#include "Button.hpp"
#include "Game.hpp"
#include "SettingsMenu.hpp"
#include "../Core/Graphics.hpp"
#include "../Core/INI.hpp"
#include "../Services/Hid.hpp"
#include "../Services/Rnx.hpp"
#include "../Services/Settings.hpp"
#include "../Services/Apm.hpp"
#include "../Services/Time.hpp"
#include "../Services/Power.hpp"
#include "../Services/App.hpp"
#include "../Services/Hid.hpp"
#include "../Types.h"

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
        void SetOverlay(std::string battery, SDL_Rect batPos, SDL_Rect clkPos);
        void UpdateSettings(u32 kDown);
        void DrawDebugText();
        
        //Interactions
        void OffsetGameIcons(u32 deltaX);
        SDL_Rect GameIconArea;
		u32 MaxColumns;
		void IncrementDashSel();
		void DecrementDashSel();
        void ActivateDash();
        void UpdateDash(u32 kDown);
		
		//Menus
		Result OpenSettings();
		Result CloseSettings();

		u8 gameRows;
		SettingsMenu *settings;
    private:
        u32 Width, Height;
        u32 lastPosX;
        bool debugInfo;
        bool GcState;
		Result lastErr;
        SDL_Rect BatPos, ClkPos, SetPos;
        SDL_Texture *Wallpaper;
        SDL_Texture *LockScreen;
        SDL_Texture *Battery;
        std::vector<Button*> Buttons;
		std::vector<Game*> Games;
};