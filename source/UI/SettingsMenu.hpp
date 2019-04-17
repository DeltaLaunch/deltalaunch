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
#include <SDL2/SDL_ttf.h>
#include <switch.h>
#include "Button.hpp"
#include "Game.hpp"
#include "Menu.hpp"
#include "Panel.hpp"
#include "../Core/Graphics.hpp"
#include "../Services/Time.hpp"
#include "../Services/Power.hpp"
#include "../Services/Settings.hpp"
#include "../Services/Hid.hpp"
#include "../Types.h"

class SettingsMenu: public Menu
{
    public:
        SettingsMenu(TTF_Font *fontHdr, TTF_Font *fontBdy, SDL_Rect pos);
        ~SettingsMenu();
        //Draw/set
        void DrawButtons();
		
		//Overrides
		void Update(u32 kDown);
		void Activate();
		void Back();
        
        //Options
        static void GameLookFeel();
        static void LockScreenToggle();
        static void VrModeToggle();
        static void UpdateConsole();
        static void EditProfilePic();
		
		u8 gameRows;
        
    private:
        u32 panX, panY;
        u32 Width, Height;
		TTF_Font *smallFont;
        SDL_Texture *bg;
        std::vector<Panel*> Panels;
};