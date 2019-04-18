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
#include "../Core/Graphics.hpp"
#include "App.hpp"
#include <string>
#include <malloc.h>
#include <switch.h>

class Account
{
    public:
        static s32 GetUserCount();
		static u128 GetActiveAccount();
        static Result SetCustomProfileImage(std::string filename);
        static u128 GetFirstAccount();
        static u128 TryGetUser();
        static SDL_Texture *GetProfileImage(u128 userID);
		static Result SetProfilePicture(u128 userId, u8 *jpg, size_t jpgSize);
};