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
#include <switch.h>
#include "Button.hpp"
#include "Draw.hpp"
#include "Renderer.hpp"
#include "Menu.hpp"
#include "../Hid.hpp"
#include "../Services/Rnx.hpp"
#include "../Services/Settings.hpp"
#include "../Services/Apm.hpp"

class Dashboard
{
    public:
        Dashboard(Renderer *rend, u32 width, u32 height, std::string font);
        ~Dashboard();
        void DrawWallpaper();
        void SetWallpaper(std::string lay0, std::string lay1, std::string lay2);
        void DrawLockScreen();
        void SetLockScreen(std::string image);
        void DrawButtons();
        void DrawMenus();
        void DrawDebugText();
        void ToggleDebug(){ debugInfo = !debugInfo; }
        void AddButton(Button *button);
        Result OpenMenu(std::string name);
        Result CloseMenus();
        void AddMenu(Menu *menu);
        bool IsMenuOpen;
    private:
        u32 Width, Height;
        bool debugInfo;
        Renderer *Rend;
        TTF_Font *Font;
		Result lastErr;
        SDL_Texture *Wallpaper;
        SDL_Texture *LockScreen;
        std::vector<Button*> Buttons;
		std::vector<Menu*> Menus;
};