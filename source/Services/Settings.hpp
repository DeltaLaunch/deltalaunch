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
#include <switch.h>
#include <vector>
#include <experimental/filesystem>

enum SelectType{
	SELECT_OUTLINE,
	SELECT_SIZEDIFF,
    SELECT_PSSTYLE
};

enum GameAreaType{
    GAMEAREA_FULL,
    GAMEAREA_MINI
};

class Settings
{
    public:
        static SelectType gameSelType;
        static GameAreaType gameAreaType;
        static std::string GetFirmwareVersion();
        static std::string GetDeviceNickname();
        static u64 GetSystemLangCode();
        static void SetDeviceNickname(std::string nick);
		static std::string GetSerialNumber();
        static u8 GetLockScreenFlag();
        static void SetLockScreenFlag(u8 flag);
        static u8 GetAudioVolume();
        static void SetAudioVolume(u8 vol);
        static std::string GetCurrentTheme();
        static size_t GetMemAvail();
        static size_t GetMemUsed();
        static u32 currentTheme;
};