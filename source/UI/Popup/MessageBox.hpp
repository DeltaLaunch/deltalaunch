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
#include <vector>
#include "../../Core/Graphics.hpp"
#include "../../Services/Hid.hpp"
#include "../Menus/Button.hpp"

class MessageBox
{
    public:
        void Show(std::string title, std::string body) {
            Title = title;
            Body = body;
            visible = true;
        }
        
        void Update() {
            SDL_Rect p;
            
            p.x = p.y = 0;
            p.w = Graphics::GetWinWidth();
            p.h = Graphics::GetWinHeight();
            Graphics::Rectangle(p, 0x50);
            Graphics::BorderedRectangle(pos, Graphics::GetDefaultButCol(), 0xFF, 5);
            Graphics::Rectangle(pos.x+10, pos.y+50, pos.w-20, 3, 0xFF);
            Graphics::DrawText(FNT_Small, pos.x+20, pos.y+12, Title, 0xFFFFFFFF, pos.w);
            Graphics::DrawText(FNT_Small, pos.x+15, pos.y+80, Body, 0xFFFFFFFF, pos.w);
            Graphics::DrawButton(Buttons[0]->Pos, Buttons[0]->Text, BTN_Unselected);
            Graphics::DrawButton(Buttons[1]->Pos, Buttons[1]->Text, BTN_Unselected);
            if(Hid::Input & KEY_LEFT) selected = (selected) ? 0 : 1;
            if(Hid::Input & KEY_RIGHT) selected = (!selected) ? 0 : 1;
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
            Buttons.push_back(new Button("Ok", pos.x+50, pos.y+200, 20, 10, 0x888888FF, nullptr));
            Buttons.push_back(new Button("Cancel", pos.x+80, pos.y+200, 20, 10, 0x888888FF, nullptr));
            selected = 0;
            visible = false;
        };
        MessageBox(const MessageBox&);
        MessageBox& operator=(const MessageBox&);
        SDL_Rect pos;
        bool visible;
        u8 selected;
        std::string Title, Body;
        std::vector<Button*> Buttons;
};