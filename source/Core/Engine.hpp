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
#include <string>
#include <vector>
#include <tuple>
#include <dirent.h>
#include <switch.h>
#include <experimental/filesystem>

#include "Threads.hpp"
#include "ThreadManager.hpp"
#include "../Core/Graphics.hpp"
#include "../Core/Audio.hpp"
#include "../Core/INI.hpp"
#include "../Core/Memory.hpp"
#include "../UI/Dashboard.hpp"
#include "../UI/Forms.hpp"
#include "../UI/Game.hpp"
#include "../Services/Hid.hpp"
#include "../Services/App.hpp"
#include "../Services/Power.hpp"
#include "../Services/Settings.hpp"

#define BACKGROUND_LAYERS 99

enum EngineState {
	STATE_LOCKSCREEN,
	STATE_DASHBOARD,
	STATE_SETTINGS
};

class Engine
{
    public:
        Engine(u32 width, u32 height, size_t heapSize);
        ~Engine();
        void Initialize();
        void Render();
        void Clear();
        void Update();
		
        Dashboard *dash;
        FsStorage romfs;
        static EngineState State;
        bool running;
    private:
        ThreadManager *frndThread;
		ThreadManager *samsThread;
		ThreadManager *aemThread;
        SDL_Rect screenPos; 
};