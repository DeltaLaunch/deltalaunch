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

#include "Settings.hpp"

SelectType Settings::gameSelType;
GameAreaType Settings::gameAreaType;

std::string Settings::GetFirmwareVersion() {
    SetSysFirmwareVersion firm;
    #ifdef SWITCH
    setsysInitialize();
    setsysGetFirmwareVersion(&firm);
    setsysExit();
    std::string str = std::string(firm.display_version);
    #else
    std::string str = std::string("0.0.0");
    #endif
    
    return str + " ReiNX";
}

std::string Settings::GetSerialNumber() {
    #ifdef SWITCH
	char serial[0x20] = {0};
	setsysInitialize();
	setsysGetSerialNumber(serial);
	setsysExit();
    std::string str = std::string(serial);
    #else
    std::string str = std::string("XAW70000000000");
    #endif
	return str;
}

std::string Settings::GetDeviceNickname() {
    #ifdef SWITCH
    char nick[0x80] = {0};
    setsysInitialize();
    setsysGetDeviceNickname(nick);
    setsysExit();
    std::string str = std::string(nick);
    #else
    std::string str = std::string("Rei");
    #endif
    return str;
}

u8 Settings::GetLockScreenFlag() {
    u8 flag = 0;
    #ifdef SWITCH
    setsysInitialize();
    setsysGetLockScreenFlag(&flag);
    setsysExit();
    #endif
    return flag;
}

void Settings::SetLockScreenFlag(u8 flag) {
    #ifdef SWITCH
    setsysInitialize();
    setsysSetLockScreenFlag(flag);
    setsysExit();
    #endif
}

u8 Settings::GetAudioVolume() {
    u8 vol = 0;
    #ifdef SWITCH
    setsysInitialize();
    setsysGetAudioVolume(&vol);
    setsysExit();
    #endif
    return vol;
}

void Settings::SetAudioVolume(u8 vol) {
    #ifdef SWITCH
    setsysInitialize();
    setsysSetAudioVolume(vol);
    setsysExit();
    #endif
}

std::vector<std::string> Settings::GetThemeNames() {
    std::vector<std::string> names;
    for (const auto & entry : std::experimental::filesystem::v1::directory_iterator("/Themes")) {
        names.push_back(entry.path().string());
	}
    return names;
}