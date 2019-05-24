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
#include "Button.hpp"
#include "Panel.hpp"
#include "../../Core/Graphics.hpp"

class Menu
{
    public:
		Menu(std::string title, SDL_Rect pos);
		virtual ~Menu();
		
		//Getters/Setters
        bool IsOpen() { return Visible; }
        void IncrementSelect();
		void DecrementSelect();
		u32 GetSelection() {return menuOpt;}
		void SetSelection(u16 sel) {menuOpt = sel;}
		std::string GetTitle() { return Title; }
		void AddElement(UIElement *elem) { MenuElements.push_back(elem); }
		void Show() { Visible = true; }
		void Hide() { Visible = false; }
		void SetBackground(std::string tex);
		
		//Overrides
		virtual void Update() {}
		virtual void Activate() { 
            if(!currLayer && Panels[menuOpt]->ElementCnt() > 0) 
                currLayer++;
        }
		virtual void Back() { 
            if(!currLayer) Hide();
            else currLayer--; 
        }
        
        SDL_Rect Pos;
		u32 Color;
		SDL_Texture *Sprite;
		u16 menuOpt, currLayer;
		bool running;		
    protected:
        std::string Title;
        bool Visible;
        std::vector<UIElement*> MenuElements;
        std::vector<Panel*> Panels;
};