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

#include "UI/Canvas.hpp"

u32 __nx_applet_type = AppletType_SystemApplet;

#define INNER_HEAP_SIZE 0x1000000
size_t nx_inner_heap_size = INNER_HEAP_SIZE;
char   nx_inner_heap[INNER_HEAP_SIZE];

void __libnx_initheap(void)
{
    void*  addr = nx_inner_heap;
    size_t size = nx_inner_heap_size;

    extern char* fake_heap_start;
    extern char* fake_heap_end;

    fake_heap_start = (char*)addr;
    fake_heap_end   = (char*)addr + size;
}

void __attribute__((weak)) __appInit(void)
{
    Result rc;
    
    // Initialize default services.
    rc = smInitialize();
    if (R_FAILED(rc))
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM));
    
    rc = nvInitialize();
    if (R_FAILED(rc))
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_BadGfxInit));
    
    rc = timeInitialize();
    if (R_FAILED(rc))
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_Time));

    rc = appletInitialize();
    if (R_FAILED(rc))
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_AM));

    rc = hidInitialize();
    if (R_FAILED(rc))
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_HID));
    
    rc = fsInitialize();
    if (R_FAILED(rc)) 
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));
    
    rc = nsInitialize();
	if (R_FAILED(rc)) 
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_NotInitialized));
    
    romfsInit();
    fsdevMountSdmc();
}

void __attribute__((weak)) __appExit(void)
{
    // Cleanup default services.
    timeExit();
    hidExit();
    nsExit();
    appletExit();
    nvExit();
    smExit();
    fsExit();
    romfsExit();
    fsdevUnmountAll();
}

void qlaunchLoop() {
    Canvas canvas(1280, 720);

	//Render loop
    while (true)
    {
        canvas.Clear();
        canvas.Update();
        canvas.Render();
    }
}

//Main loop
int main(int argc, char* argv[])
{    
    //appletUnlockForeground();
    //appletRequestForeground();
    
    //appletSetHandlesRequestToDisplay(true);
    // Retrieve the default window
    
    qlaunchLoop();
    
    return 0;
}