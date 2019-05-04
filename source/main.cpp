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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <switch.h>

#include "Core/Engine.hpp"

#ifdef SWITCH
extern "C"{
    u32 __nx_applet_type = AppletType_SystemApplet;
    
    void userAppInit(void);
    void userAppExit(void);
}

void userAppInit(void) {
    romfsInit();
}

void userAppExit(void) {
    romfsExit();
}
#endif

int main(int argc, char* argv[]) {
    //Qlaunch loop
    Engine eng(1280, 720);
    eng.Initialize();
    while (eng.running) {
        eng.Clear();
        eng.Update();
        eng.Render();
    }
    
    return 0;
}