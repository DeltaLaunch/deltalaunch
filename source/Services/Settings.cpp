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
    setsysInitialize();
    setsysGetFirmwareVersion(&firm);
    setsysExit();
    std::string str = std::string(firm.display_version);
    return str + " ReiNX";
}

std::string Settings::GetSerialNumber() {
	char serial[0x20] = {0};
	setsysInitialize();
	setsysGetSerialNumber(serial);
	setsysExit();
	return std::string(serial);
}

std::string Settings::GetDeviceNickname() {
    char nick[0x80] = {0};
    setsysInitialize();
    setsysGetDeviceNickname(nick);
    setsysExit();
    return std::string(nick);
}

u8 Settings::GetLockScreenFlag() {
    u8 flag;
    setsysInitialize();
    setsysGetLockScreenFlag(&flag);
    setsysExit();
    return flag;
}

void Settings::SetLockScreenFlag(u8 flag) {
    setsysInitialize();
    setsysSetLockScreenFlag(flag);
    setsysExit();
}

u8 Settings::GetAudioVolume() {
    u8 vol;
    setsysInitialize();
    setsysGetAudioVolume(&vol);
    setsysExit();
    return vol;
}

void Settings::SetAudioVolume(u8 vol) {
    setsysInitialize();
    setsysSetAudioVolume(vol);
    setsysExit();
}