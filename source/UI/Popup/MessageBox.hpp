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
#include <switch.h>
#include "../../Core/Graphics.hpp"
#include "../../Services/Hid.hpp"

class MessageBox
{
    public:
        void Show(std::string title, std::string body) {
            visible = true;
        }
        
        void Update() {
            Graphics::Rectangle(pos, Graphics::GetDefaultButCol());
            if(Hid::Input & KEY_A){
                visible = false;
            }
        }
        
        bool IsOpen() { return visible; }
        
        static MessageBox* getInstance() {
            if (instance == nullptr)
                instance = new MessageBox();
            return instance;
        };
        
    private:
        static MessageBox* instance;
        MessageBox() {
            u32 wid = Graphics::GetWinWidth();
            u32 height = Graphics::GetWinHeight();
            pos.w = wid/2;
            pos.h = height/2;
            pos.x = (wid/2) - (pos.w/2);
            pos.y = (height/2) - (pos.h/2);
            visible = false;
        };
        MessageBox(const MessageBox&);
        MessageBox& operator=(const MessageBox&);
        SDL_Rect pos;
        bool visible;
};