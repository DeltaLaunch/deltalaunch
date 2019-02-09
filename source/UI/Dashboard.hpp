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
#include "../Hid.hpp"
#include "../Debug.hpp"

class Dashboard
{
    public:
        Dashboard(Renderer &rend, std::string fnt, u32 fntSize);
        ~Dashboard();
        void DrawWallpaper();
        void DrawButtons();
        void DrawDebugText();
        void AddButton(Button button);
        void Update();
        void SetWallpaper(std::string lay0, std::string lay1, std::string lay2);
    private:
        Renderer Rend;
        TTF_Font *smallFnt;
        Debug *dbg;
        Result lastErr;
        std::string Lay0, Lay1, Lay2;
        std::vector<Button> Buttons;
};