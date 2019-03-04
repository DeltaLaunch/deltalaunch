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
#include <vector>
#include <switch.h>

#include "../UI/Renderer.hpp"
#include "../UI/Draw.hpp"
#include "../UI/INI.hpp"
#include "../UI/Dashboard.hpp"
#include "../UI/Button.hpp"
#include "../UI/Game.hpp"
#include "../UI/Menu.hpp"
#include "../Services/Hid.hpp"
#include "Threads.hpp"
#include "../Services/App.hpp"
#include "../Services/Power.hpp"

#define BACKGROUND_LAYERS 99

class Engine
{
    enum EngineState {
        LOCKSCREEN,
        DASHBOARD,
        SETTINGS
    };
    public:
        Engine(u32 width, u32 height, void *heapAddr, size_t heapSize);
        ~Engine();
        void Render();
        void Clear();
        void Update();
        void GetInputs();
		
        Renderer mRender;
        Dashboard *dash;
        Mix_Music *bgm;
        u32 lastPosX;
    private:
        Thread thread;
        EngineState State;
        std::string baseThemeDir;
        u32 Width, Height;
        void *HeapAddr;
        size_t HeapSize;
        bool GcState;
};