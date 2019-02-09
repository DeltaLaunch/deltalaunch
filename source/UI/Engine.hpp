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
#include <string>
#include <algorithm>
#include <switch.h>

#include "Renderer.hpp"
#include "Draw.hpp"
#include "INI.hpp"
#include "Dashboard.hpp"
#include "Button.hpp"
#include "Menu.hpp"
#include "../Hid.hpp"
#include "../Services/App.hpp"
#include "../Services/Power.hpp"

class Engine
{
    public:
        Engine(uint32_t width, uint32_t height);
        ~Engine();
        void Render();
        void Clear();
        void Update();
        Renderer mRender;
        Dashboard *dash;
        Mix_Music *bgm;
    private:
        std::string baseThemeDir;
        std::string bgLay0, bgLay1, bgLay2;
        std::vector<Menu> Menus;
        bool debugInfo;
};