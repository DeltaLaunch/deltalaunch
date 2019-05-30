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

enum MsgType {
    MSGBOX_OK,
    MSGBOX_YESNO,
};

enum DialogResult {
    RESULT_OK,
    RESULT_ABORT,
    RESULT_NO,
    RESULT_YES
};

class MessageBox
{
    public:
        void Show(std::string title, std::string body, MsgType type = MSGBOX_OK){
            Title = title;
            Body = body;
            msgType = type;
            
            //Draw backdrop
            screen.x = screen.y = 0;
            screen.w = Graphics::GetWinWidth();
            screen.h = Graphics::GetWinHeight();
            
            //Update form
            while(Update());
        }
        
        void SetTexture(std::string file) {
            bgTex = Graphics::CreateTexFromString(file);
        }
        
        static MessageBox* getInstance() {
            if (instance == nullptr)
                instance = new MessageBox();
            return instance;
        };
        
        DialogResult MsgResult;
        
    private:
        static MessageBox* instance;
        
        MessageBox() {
            u32 wid = Graphics::GetWinWidth();
            u32 height = Graphics::GetWinHeight();
            pos.w = wid/2;
            pos.h = height/2;
            pos.x = (wid/2) - (pos.w/2);
            pos.y = (height/2) - (pos.h/2);
            u32 butW = 150, butH = 50;
            Buttons.push_back(new Button("Ok", pos.x+pos.w-butW-25, pos.y+pos.h-butH-25, butW, butH, 0x888888FF, nullptr));
            Buttons.push_back(new Button("No", pos.x+pos.w-butW-25, pos.y+pos.h-butH-25, butW, butH, 0x888888FF, nullptr));
            Buttons.push_back(new Button("Yes", pos.x+pos.w-((butW+25)*2), pos.y+pos.h-butH-25, butW, butH, 0x888888FF, nullptr));
            selected = 0;
        };
        
        int Update() {
            
            Hid::KeyProcess();
            Hid::TouchProcess();
            
            Graphics::RenderTexture(bgTex, pos);
            Graphics::DrawText(Fonts::FONT_SMALL, pos.x+25, pos.y+18, Title, 0xFFFFFFFF, pos.w);
            Graphics::DrawText(Fonts::FONT_SMALL, pos.x+25, pos.y+80, Body, 0xFFFFFFFF, pos.w);
            
            switch(msgType) {
                case MSGBOX_OK:
                {
                    Graphics::DrawButton(Buttons[0]->Pos, Buttons[0]->Text, BTN_Selected_Lay1);
                    break;
                }
                case MSGBOX_YESNO:
                {
                    Graphics::DrawButton(Buttons[1]->Pos, Buttons[1]->Text, selected ? BTN_Unselected : BTN_Selected_Lay1);
                    Graphics::DrawButton(Buttons[2]->Pos, Buttons[2]->Text, selected ? BTN_Selected_Lay1 : BTN_Unselected);
                    break;
                }
            }
            
            if(Hid::Input & KEY_LEFT) selected = (msgType == MSGBOX_YESNO) ? 1 : 0; //TODO: ugly. plz fix
            if(Hid::Input & KEY_RIGHT) selected = 0;
            if(Hid::Input & KEY_A){
                MsgResult = (msgType == MSGBOX_YESNO) ? (DialogResult)(RESULT_NO + selected) : RESULT_OK;
                return 0;
            }
            Graphics::Render();
            return 1;
        }
        
        MessageBox(const MessageBox&);
        MessageBox& operator=(const MessageBox&);
        SDL_Rect pos;
        SDL_Rect screen;
        u8 selected;
        SDL_Texture *bgTex;
        MsgType msgType;
        std::string Title, Body;
        std::vector<Button*> Buttons;
};