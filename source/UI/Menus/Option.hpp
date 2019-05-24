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
#include "UIElement.hpp"

class Option: public UIElement
{
    public:
        Option(std::string optName, std::vector<std::string> opts, u32 x, u32 y, u32 w, u32 h, u32 col, u32 defaultVal, std::function<Result()> callback) : UIElement(x, y, w, h) {
            Callback = callback;
            Opts = opts;
            optIndex = defaultVal;
            Text = optName;
            Properties = ELEM_Option;
        };
        
        Option(std::string optName, std::string opt, u32 x, u32 y, u32 w, u32 h, u32 col, u32 defaultVal, std::function<Result()> callback) : UIElement(x, y, w, h) {
            Callback = callback;
            Opts.push_back(opt);
            optIndex = defaultVal;
            Text = optName;
            Properties = ELEM_Option;
        };
        
        ~Option() {
            Opts.clear();
        };
        
        void Draw(SDL_Rect pos, bool selected) override {
            SDL_Rect p = pos;
            p.x += Pos.x; p.y += Pos.y;
            p.w = Pos.w; p.h = Pos.h;
            Graphics::DrawOption(p, Text, !Opts.empty() ? Opts.at(optIndex) : "", selected);
        }
        
        Result Run() override {
            Result rc = 0;
            if(Callback != nullptr && R_SUCCEEDED(rc = Callback())) 
                optIndex = ((!Opts.empty() && optIndex >= Opts.size()-1) ? 0 : optIndex+1);
            return rc;
        }
        
        u32 GetOptIndex() { 
            return optIndex; 
        }
        
        void SetOption(u32 ind, std::string text) override {
            Opts[ind] = text;
        }
        
    private:
        u32 optIndex;
        std::vector<std::string> Opts;
};