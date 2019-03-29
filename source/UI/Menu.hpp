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
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <switch.h>
#include "INI.hpp"
#include "Button.hpp"
#include "../Core/Graphics.hpp"

class Menu
{
    public:
		Menu(std::string title, TTF_Font *fontHdr, TTF_Font *fontBdy, SDL_Rect pos);
		virtual ~Menu();
		
		//Getters/Setters
        bool IsOpen() { return Visible; }
        void IncrementSelect();
		void DecrementSelect();
		u32 GetSelection() {return menuOpt;}
		void SetSelection(u16 sel) {menuOpt = sel;}
		std::string GetTitle() { return Title; }
		void AddButton(Button *button) { Buttons.push_back(button); }
		void Show() { Visible = true; }
		void Hide() { Visible = false; }
		void SetBackground(std::string tex);
		
		//Overrides
		virtual void Update(u32 kDown) {}
		virtual void Activate() {}
		virtual void Back() {}
        
        SDL_Rect Pos;
		u32 Color;
		SDL_Texture *Sprite;
		TTF_Font *FontHdr;
        TTF_Font *FontBdy;
		u16 menuOpt, currLayer;
		bool running;
	private:
		std::string Title;
        bool Visible;
    protected:
        std::vector<Button*> Buttons;
};