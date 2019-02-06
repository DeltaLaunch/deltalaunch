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
#include <vector>
#include <switch.h>
#include "Button.hpp"
#include "Draw.hpp"
#include "Renderer.hpp"

class Dashboard
{
    public:
        Dashboard();
		~Dashboard();
        void DrawWallpaper(std::string bgLay0, std::string bgLay1, std::string bgLay2, std::string dir, Renderer rend);
        void DrawButtons(Renderer rend);
        void AddButton(Button button);
	private:
		std::vector<Button> Buttons;
};