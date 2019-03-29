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

#include "App.hpp"

AppletHolder App::currentApplication;
u32 App::appletSelectInd;
u32 App::gameSelectInd;
u8 App::dashLayer;

u32 App::lastAeCmd;

/*
*	Application
*/
Result App::LaunchGame(u64 tid, u128 userID) {
    AppletStorage aStore;
    Result rc = 0;
    
	rc = appCreate(&currentApplication, tid, Create_App);
	if(R_FAILED(rc)) {
        ShowError("Error launching game", "Error initializing handle", rc);
    }
	
    rc = appletUnlockForeground();
	if(R_FAILED(rc)) {
        ShowError("Error launching game", "Error unlocking foreground", rc);
    }
    
	struct InData{
        u32 code;
        u8 unk1;
        u8 pad[3];
        u128 id;
        u8 unk2[0x70];
    } __attribute__((packed));
    
    InData indata = {0};
    indata.code = 0xC79497CA;
    indata.unk1 = 1;
    indata.id = userID;
	
    rc = appletCreateStorage(&aStore, sizeof(InData));
    if(R_FAILED(rc)) {
        ShowError("Error launching game", "Error initializing arg storage", rc);
    }

    rc = appletStorageWrite(&aStore, 0, &indata, sizeof(InData));
    if(R_FAILED(rc)) {
        ShowError("Error launching game", "Error writing arg storage", rc);
    }
	
    rc = appletHolderPushInLaunchParam(&currentApplication, AppletLaunchParameterKind_PreselectedUser, &aStore);
	if(R_FAILED(rc)) {
        ShowError("Error launching game", "Error appletHolderPushInLaunchParam", rc);
    }
    
    rc = appletHolderStart(&currentApplication);
    if(R_FAILED(rc)) {
        ShowError("Error launching game", "Lookup errorcode for more info", rc);
    }
	
	rc = appRequestForApplicationToGetForeground(&currentApplication);
	if(R_FAILED(rc)) {
        ShowError("Error launching game", "Error appRequestForApplicationToGetForeground", rc);
    }
	currentApplication.active = true;
    appletHolderJoin(&currentApplication);
    appletHolderClose(&currentApplication);
	appletStorageClose(&aStore);
    
    return rc;
}

Result App::GetTitleIds(std::vector<u64> &tids) {
    Result rc;
    size_t size = 0;
    NsApplicationRecord rec[2000];
    rc = nsInitialize();
    rc = nsListApplicationRecord(rec, 2000, 0, &size);
    for(int i = 0; i < (int)size; i++) {
        tids.push_back(rec[i].titleID);
    }
    nsExit();
    return rc;
}

NsApplicationControlData App::GetGameControlData(u64 tid, u8 flag) {
	NsApplicationControlData buffer;
	size_t s = 0;
    nsInitialize();
	nsGetApplicationControlData(flag, tid, &buffer, 0x20000, &s);
    nsExit();
	return buffer;
}

bool App::IsGamecardInserted() {
    FsDeviceOperator opt;
    bool b = false;
    fsOpenDeviceOperator(&opt);
    fsDeviceOperatorIsGameCardInserted(&opt, &b);
    return b;
}

/*
*	Applet
*/
Result App::LaunchAlbum(u8 arg, bool startupSound) {
	AppletHolder h;
	LibAppletArgs args;
	AppletStorage storeIn;
	Result rc = 0;
	
	appletCreateLibraryApplet(&h, AppletId_photoViewer, LibAppletMode_AllForeground);
	libappletArgsCreate(&args, 0);
    libappletArgsSetPlayStartupSound(&args, startupSound);
    libappletArgsPush(&args, &h);
	
	u8 stindata = arg;
	rc = appletCreateStorage(&storeIn, sizeof(stindata));
	if(R_FAILED(rc)) {
        appletStorageClose(&storeIn);
        ShowError("Error launching player select", "Error creating storage.", rc);
    }
	
    rc = appletStorageWrite(&storeIn, 0, &stindata, sizeof(stindata));
    if(R_FAILED(rc)) {
        appletStorageClose(&storeIn);
        ShowError("Error launching player select", "Error writing storage.", rc);
    }
    appletHolderPushInData(&h, &storeIn);
	
	rc = appletHolderStart(&h);
    appletHolderJoin(&h);
	
	appletStorageClose(&storeIn);
    appletHolderClose(&h);
	
    return rc;
}

u128 App::LaunchPSelect() {
	AppletHolder h;
    AppletStorage storeIn, storeOut;
	u128 player = 0;
    Result rc = 0;
	LibAppletArgs args;
    
	rc = appletCreateLibraryApplet(&h, AppletId_playerSelect, LibAppletMode_AllForeground);
	if(R_FAILED(rc)) {
        ShowError("Error launching player select", "Error creating lib applet.", rc);
    }
	
    libappletArgsCreate(&args, 0);
    libappletArgsPush(&args, &h);
    
	u8 stindata[0xa0] = { 0 };
	
    rc = appletCreateStorage(&storeIn, sizeof(stindata));
	if(R_FAILED(rc)) {
        appletStorageClose(&storeIn);
        ShowError("Error launching player select", "Error creating storage.", rc);
    }
	
    rc = appletStorageWrite(&storeIn, 0, stindata, sizeof(stindata));
    if(R_FAILED(rc)) {
        appletStorageClose(&storeIn);
        ShowError("Error launching player select", "Error writing storage.", rc);
    }
    appletHolderPushInData(&h, &storeIn);
	
	rc = appletHolderStart(&h);
	appletHolderJoin(&h);
    if(R_FAILED(rc)) {
        ShowError("Error launching player select", "Error starting applet.", rc);
    }
    else {
        appletHolderPopOutData(&h, &storeOut);
        u8 buf[0x18] = {0};
        appletStorageRead(&storeOut, 0, buf, 0x18);
        player = *(u128*)&buf[8];
    }
    appletStorageClose(&storeIn);
    appletStorageClose(&storeOut);
    appletHolderClose(&h);
	
	appletRequestForeground();
    
    return player;
}

Result App::LaunchShop() {
	AppletHolder h;
	LibAppletArgs args;
    AppletStorage storeIn, storeOut;
    Result rc = 0;
	
	appletCreateLibraryApplet(&h, AppletId_shop, LibAppletMode_AllForeground);
	libappletArgsCreate(&args, 1);
    libappletArgsPush(&args, &h);
    
    rc = appletCreateStorage(&storeIn, 0x2000);
    if(R_FAILED(rc)) {
        ShowError("Error launching eshop", "Error initializing arg storage", rc);
    }

    char indata[0x2000] = {0};
	sprintf (indata, "eshop://%s", "success" );

    rc = appletStorageWrite(&storeIn, 0, indata, 0x2000);
    if(R_FAILED(rc)) {
        ShowError("Error launching eshop", "Error writing arg storage", rc);
    }
    appletHolderPushInData(&h, &storeIn);
    
	rc = appletHolderStart(&h);
	appletHolderJoin(&h);
    if(R_FAILED(rc)) {
        ShowError("Error launching eshop", "Error starting applet.", rc);
    }
    else {
        appletHolderPopOutData(&h, &storeOut);
        u8 buf[0x1010] = {0};
        appletStorageRead(&storeOut, 0, buf, sizeof(buf));
        //TODO
    }
    appletStorageClose(&storeIn);
    appletStorageClose(&storeOut);
    appletHolderClose(&h);
	
    return rc;
}

Result App::LaunchWebsite(std::string url) {
    AppletHolder h;
    AppletStorage aStore;
    LibAppletArgs aArgs;
    Result rc = 0;
    rc = appletCreateLibraryApplet(&h, AppletId_web, LibAppletMode_AllForeground);
    if(R_FAILED(rc)) {
        ShowError("Error launching browser", "Error initializing applet", rc);
    }
    libappletArgsCreate(&aArgs, 0x50000);
    libappletArgsPush(&aArgs, &h);
    rc = appletCreateStorage(&aStore, 8192);
    if(R_FAILED(rc)) {
        ShowError("Error launching browser", "Error initializing arg storage", rc);
    }

    u8 indata[0x2000] = {0};
    *(u64*)&indata[4] = 0x1000D00000005;
    *(u64*)&indata[17] = 0xC000001;
    *(u8*)&indata[16] = 1;
    *(u16*)indata = 2;
    strcpy((char*)&indata[25], url.c_str());

    rc = appletStorageWrite(&aStore, 0, indata, 0x2000);
    if(R_FAILED(rc)) {
        ShowError("Error launching browser", "Error writing arg storage", rc);
    }
    appletHolderPushInData(&h, &aStore);
    rc = appletHolderStart(&h);
    if(R_FAILED(rc)) {
        ShowError("Error launching browser", "Lookup errorcode for more info", rc);
    }
    appletHolderJoin(&h);
    LibAppletExitReason e = appletHolderGetExitReason(&h);
    if(e != LibAppletExitReason_Normal) {
        ShowError("Browser Error", "Lookup errorcode for more info", rc);
    }
    appletHolderClose(&h);
    appletStorageClose(&aStore);

    return rc;
}

Result App::ShowError(std::string errText, std::string details, Result rc) {
	AppletHolder h;
    AppletStorage errStor;
    LibAppletArgs args;

    appletCreateLibraryApplet(&h, AppletId_error, LibAppletMode_AllForeground);
    libappletArgsCreate(&args, 1);
    libappletArgsPush(&args, &h);
    appletCreateStorage(&errStor, 0x1018);
    u8 argBuf[0x1018] = {0};
    argBuf[0] = 1;

    *(u64*)&argBuf[8] = (((rc & 0x1ffu) + 2000) | (((rc >> 9) & 0x1fff & 0x1fffll) << 32));
    strcpy((char*) &argBuf[24], errText.c_str());
    strcpy((char*) &argBuf[0x818], details.c_str());
    appletStorageWrite(&errStor, 0, argBuf, 0x1018);
    appletHolderPushInData(&h, &errStor);

    appletHolderStart(&h);
    appletHolderJoin(&h);
	appletHolderClose(&h);
    
	return 0;
}

Result App::LaunchHbl() {
    //TODO
    return 0;
}

/*
*	Misc
*/
Result App::CommandHandler(u32 cmd) {
    switch(cmd) {
        case CMD_Home:
        {
            if(currentApplication.active) {
                appRequestExit(&currentApplication);
                currentApplication.active = false;
            }
            else {
                gameSelectInd = 0;
                appletSelectInd = 0;
                dashLayer = 0;
            }
            break;
        }
        case 14:
        {
            //
            break;
        }
        case CMD_Sleepmode:
        {
            Power::Sleep();
            break;
        }
        case CMD_Shutdown:
        {
            Power::Shutdown();
            break;
        }
        case CMD_Reboot:
        {
            Power::Reboot();
            break;
        }
    }
    
    return 0;
}