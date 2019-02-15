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

void App::GetList() {
    NsApplicationRecord records[0x100];
    size_t entries = 0;
    nsListApplicationRecord(records, 0x100, 0, &entries);
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
    return 0;
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
    //LibAppletArgs aArgs;
    
    Result rc = 0;
	appCreate(&h, tid, true, Create_App);
    appRequestForApplicationToGetForeground(&h);
    
    rc = appletCreateStorage(&aStore, 0x88);
    if(R_FAILED(rc)) {
        ShowError("Error launching browser", "Error initializing arg storage", rc);
    }
    
    u8 indata[0x88] = {0};
    *(u32*)&indata[0] = 0xC79497CA;
    *(u8*)&indata[4] = 1;
    *(u128*)&indata[8] = userID;
    
    rc = appletStorageWrite(&aStore, 0, indata, 0x88);
    if(R_FAILED(rc)) {
        ShowError("Error launching game", "Error writing arg storage", rc);
    }
    appletHolderPushInData(&h, &aStore);
    rc = appletHolderStart(&h);
    if(R_FAILED(rc)) {
        ShowError("Error launching browser", "Lookup errorcode for more info", rc);
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