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
#include <string>
#include <functional>
#include <vector>
#include <switch.h>

class Option
{
    public:
        Option(std::string optName, std::vector<std::string> opts, u32 x, u32 y, u32 w, u32 h, u32 col, u32 defaultVal, std::function<Result()> callback) {
            Pos.x = x;
            Pos.y = y;
            Pos.w = w;
            Pos.h = h;
            Callback = callback;
            Opts = opts;
            optIndex = defaultVal;
            Text = optName;
        };
        ~Option() {
            Opts.clear();
        };
        
        void Run() { 
            if(Callback != nullptr && R_SUCCEEDED(Callback())) 
                optIndex = ((!Opts.empty() && optIndex >= Opts.size()-1) ? 0 : optIndex+1);
        }
        bool HasFunc() { return Callback != nullptr; }
        
        u32 GetOptIndex() { return optIndex; }
        std::string GetOptText() { return !Opts.empty() ? Opts.at(optIndex) : ""; };
        
        SDL_Rect Pos;
        std::string Text;
    private:
        u32 optIndex;
        std::vector<std::string> Opts;
        std::function<Result()> Callback;
};