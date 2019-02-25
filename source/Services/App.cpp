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

Result App::GetTitleIds(std::vector<u64> &tids) {
    Result rc;
    size_t size = 0;
    NsApplicationRecord rec[2000];
    rc = nsInitialize();
    rc = nsListApplicationRecord(rec, 2000, 0, &size);
    for(int i = 0; i < size; i++) {
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

Result App::LaunchAlbum() {
	AppletHolder h;
	LibAppletArgs args;
	
	appletCreateLibraryApplet(&h, AppletId_photoViewer, LibAppletMode_AllForeground);
	libappletArgsCreate(&args, 1);
    libappletArgsPush(&args, &h);
	appletHolderStart(&h);
    appletHolderJoin(&h);
    appletHolderClose(&h);
    libappletRequestHomeMenu();
    return 0;
}

u128 App::LaunchPSelect() {
	AppletHolder h;
    AppletStorage storage;
	u128 player;
    Result rc = 0;
    
	appletCreateLibraryApplet(&h, AppletId_playerSelect, LibAppletMode_AllForeground);
    u8 argBuf[0xA0] = {0};
    appletCreateStorage(&storage, sizeof(argBuf));
    rc = appletStorageWrite(&storage, 0, argBuf, sizeof(argBuf));
    if(R_FAILED(rc)) {
        appletStorageClose(&storage);
        ShowError("Error launching player select", "Error writing storage.", rc);
    }
    appletHolderPushInData(&h, &storage);
	rc = appletHolderStart(&h);
    appletHolderJoin(&h);
    if(R_FAILED(rc)) {
        ShowError("Error launching player select", "Error starting applet.", rc);
    }
    else {
        appletHolderPopOutData(&h, &storage);
        u8 buf[0x18] = {0};
        appletStorageRead(&storage, 0, buf, 0x18);
        player = *(u128*)(buf+8);
    }
    appletHolderClose(&h);
    appletStorageClose(&storage);
    
    return player;
}

Result App::LaunchShop() {
	AppletHolder h;
	LibAppletArgs args;
    AppletStorage aStore;
    Result rc = 0;
	
	appletCreateLibraryApplet(&h, AppletId_shop, LibAppletMode_AllForeground);
	libappletArgsCreate(&args, 1);
    libappletArgsPush(&args, &h);
    
    rc = appletCreateStorage(&aStore, 0x2000);
    if(R_FAILED(rc)) {
        ShowError("Error launching browser", "Error initializing arg storage", rc);
    }

    u8 indata[0x2000] = {0};
    strcpy((char*)&indata[0], "eshop://");

    rc = appletStorageWrite(&aStore, 0, indata, 0x2000);
    if(R_FAILED(rc)) {
        ShowError("Error launching browser", "Error writing arg storage", rc);
    }
    appletHolderPushInData(&h, &aStore);
    rc = appletHolderStart(&h);
    if(R_FAILED(rc)) {
        ShowError("Error launching browser", "Lookup errorcode for more info", rc);
    }
    
	appletHolderStart(&h);
    appletHolderJoin(&h);
    appletHolderClose(&h);
    appletStorageClose(&aStore);
    return rc;
}

Result App::LaunchSystemApplication(u64 tid) {
	AppletHolder h;
	appCreate(&h, tid, true, Create_SystemApp);
    appRequestForApplicationToGetForeground(&h);
    appletHolderStart(&h);
    appletHolderJoin(&h);
    appletHolderClose(&h);
	return 0;
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

Result App::LaunchHbl() {
    //TODO
    return 0;
}

Result App::LaunchGame(u64 tid, u128 userID) {
    AppletHolder h;
    AppletStorage aStore;
    Result rc = 0;
    
	appCreate(&h, tid, true, Create_App);
    appletUnlockForeground();
    
    rc = appletCreateStorage(&aStore, 0x88);
    if(R_FAILED(rc)) {
        ShowError("Error launching game", "Error initializing arg storage", rc);
    }
    
    struct InData{
        u32 code;
        u8 unk1;
        u8 pad[3];
        u128 id;
        u8 unk2[0x70];
    };
    InData indata = {0};
    indata.code = 0xC79497CA;
    indata.unk1 = 1;
    indata.id = userID;
    
    rc = appletStorageWrite(&aStore, 0, &indata, 0x88);
    if(R_FAILED(rc)) {
        ShowError("Error launching game", "Error writing arg storage", rc);
    }
    appletHolderPushInData(&h, &aStore);
    appRequestForApplicationToGetForeground(&h);
    rc = appletHolderStart(&h);
    if(R_FAILED(rc)) {
        ShowError("Error launching game", "Lookup errorcode for more info", rc);
    }
    appletHolderJoin(&h);
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
	
	return 0;
}