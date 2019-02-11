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

#include "Apm.hpp"

float Apm::GetCpuClock() {
    u32 profile = GetProfile();
    float cpu = 0;
    switch(profile){
        case 0x00010002:
        case 0x00020002:
            cpu = 1224;
            break;
        default:
            cpu = 1020;
            break;
    }
    return cpu;
}

float Apm::GetGpuClock() {
    u32 profile = GetProfile();
    float gpu = 0;
    switch(profile){
        case 0x00010001:
            gpu = 768;
            break;
        case 0x00010002:
            gpu = 691.2;
            break;
        case 0x00020000:
        case 0x00020002:
            gpu = 230.4;
            break;
        case 0x00020001:
        case 0x00020005:
            gpu = 307.2;
            break;
        case 0x00020003:
            gpu = 307;
            break;
        case 0x92220007:
        case 0x92220008:
            gpu = 460.8;
            break;
        default:
            gpu = 384;
            break;
    }
    return gpu;
}

float Apm::GetMemClock() {
    u32 profile = GetProfile();
    float mem = 0;
    switch(profile) {
        case 0x00020003:
        case 0x00020004:
        case 0x92220008:
            mem = 1331.2;
            break;
        case 0x00020005:
        case 0x00020006:
            mem = 1065.6;
            break;
        default:
            mem = 1600;
            break;
    }
    return mem;
}

u32 Apm::GetProfile() {
    u32 profile = 0;
    apmInitialize();
    apmGetPerformanceConfiguration(0, &profile);
    apmExit();
    return profile;
}