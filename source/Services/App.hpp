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
#include <string.h>
#include <stdlib.h>
#include <switch.h>

class App
{
    public:
        static void GetList();
		static Result LaunchAlbum();
		static Result LaunchShop();
		static Result LaunchSystemApplication(u64 tid);
        static Result LaunchWebsite(std::string url);
        static Result LaunchHbl();
        static Result LaunchGame(u64 tid, u128 userID);
		static Result ShowError(std::string text1, std::string text2, Result rc);
};