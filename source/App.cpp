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

Result App::LaunchApplet(AppletId app, LibAppletMode mode) {
	AppletHolder h;
	appletCreateLibraryApplet(&h, app, mode);
	appletHolderStart(&h);
}

Result App::LaunchApplication(u64 tid) {
	AppletHolder holder;
	//appCreateApplication(&holder, tid);
    //appRequestForApplicationToGetForeground(&holder);
    appletHolderJoin(&holder);
}

Result App::LaunchWebsite(std::string url) {
    AppletHolder aHold;
    AppletStorage aStore;
    LibAppletArgs aArgs;
    nsvmInitialize();
    Result rc = appletCreateLibraryApplet(&aHold, AppletId_web, LibAppletMode_AllForeground);
    if(R_FAILED(rc)) {
        //showError("Error launching browser", "Error initializing applet", rc);
    }
    libappletArgsCreate(&aArgs, 0x50000);
    libappletArgsPush(&aArgs, &aHold);
    rc = appletCreateStorage(&aStore, 8192);
    if(R_FAILED(rc)) {
        //showError("Error launching browser", "Error initializing arg storage", rc);
    }

    u8 indata[8192] = {0};
    *(u64*)&indata[4] = 281530811285509;
    *(u64*)&indata[17] = 201326593;
    *(u8*)&indata[16] = 1;
    *(u16*)indata = 2;
    strcpy((char*)&indata[25], url.c_str());

    rc = appletStorageWrite(&aStore, 0, indata, 8192);
    if(R_FAILED(rc)) {
        //showError("Error launching browser", "Error writing arg storage", rc);
    }
    appletHolderPushInData(&aHold, &aStore);
    rc = appletHolderStart(&aHold);
    if(R_FAILED(rc)) {
        //showError("Error launching browser", "Lookup errorcode for more info", rc);
    }
    appletHolderJoin(&aHold);
    LibAppletExitReason e = appletHolderGetExitReason(&aHold);
    if(e != LibAppletExitReason_Normal) {
        //showError("Browser Error", "Lookup errorcode for more info", rc);
    }
    appletHolderClose(&aHold);
    appletStorageClose(&aStore);
    nsvmExit();
}