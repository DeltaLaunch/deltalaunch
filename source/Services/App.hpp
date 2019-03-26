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
#include <vector>
#include <stdio.h>
#include <switch.h>
#include "Power.hpp"
#include "../Types.h"

class App
{
    public:
        static Result GetTitleIds(std::vector<u64> &tids);
		static NsApplicationControlData GetGameControlData(u64 tid, u8 flag);
        static bool IsGamecardInserted();
		static Result LaunchAlbum();
        static u128 LaunchPSelect();
		static Result LaunchShop();
		static Result LaunchSystemApplication(u64 tid);
        static Result LaunchWebsite(std::string url);
        static Result LaunchHbl();
        static Result LaunchGame(u64 tid, u128 userID);
		static Result ShowError(std::string text1, std::string text2, Result rc);
        static Result CommandHandler(u32 cmd);
};