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

#include "Threads.hpp"

void Threads::DumpFile(std::string name, void *buffer, size_t size) {
	FILE *fp = fopen(name.c_str(), "wb");
	fwrite(buffer, size, 1, fp);
	fclose(fp);
}

bool Threads::FriendThread() {
	//
	return true;
}

bool Threads::SystemAppletMessage() {
	Event epop;
    Result rc = appletGetPopFromGeneralChannelEvent(&epop);
    if(rc == 0)
    {
        eventWait(&epop, U64_MAX);
        AppletStorage st;
        rc = appletGetPopFromGeneralChannel(&st);
        if(rc == 0) {
			s64 size = 0;
			appletStorageGetSize(&st, &size);
			if(size > 0) {
				struct SAMS{
					u8 magic[4];
					u32 version;
					u32 cmdID;
					u8 unk[0x3f4];
				} __attribute__((packed));
				
				SAMS sams = {0};
				appletStorageRead(&st, 0, &sams, size);
				
				switch(sams.cmdID) {
					case 1:
					{
						DumpFile("/dump_1.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
					case 2:
					{
						DumpFile("/dump_2.bin", &sams, (size_t)sizeof(SAMS)); //homemenu
						Power::Shutdown();
						break;
					}
					case 3:
					{
						Power::Sleep();
						break;
					}
					case 4:
					{
						DumpFile("/dump_4.bin", &sams, (size_t)sizeof(SAMS));//unk (launcher?)
						Power::Shutdown();
						break;
					}
					case 5:
					{
						Power::Shutdown();
						break;
					}
					case 6:
					{
						Power::Reboot();
						break;
					}
					case 7:
					{
						DumpFile("/dump_7.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
					case 8:
					{
						DumpFile("/dump_8.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
					case 9:
					{
						DumpFile("/dump_9.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
					case 10:
					{
						DumpFile("/dump_10.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
					case 11:
					{
						//update
						break;
					}
					case 12:
					{
						DumpFile("/dump_12.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
					case 13:
					{
						DumpFile("/dump_13.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
					case 14:
					{
						DumpFile("/dump_14.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
					case 15:
					{
						DumpFile("/dump_15.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
					case 16:
					{
						//homeSideMenu
						break;
					}
					case 17:
					{
						DumpFile("/dump_17.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
					case 18:
					{
						DumpFile("/dump_18.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
					case 19:
					{
						DumpFile("/dump_19.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
					case 20:
					{
						DumpFile("/dump_20.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
					case 21:
					{
						DumpFile("/dump_21.bin", &sams, (size_t)sizeof(SAMS));//unk
						Power::Shutdown();
						break;
					}
				}
			}
		}
    }
    eventClose(&epop);
    
	return true;
}

bool Threads::AeMessageThread() {
	/*u32 msg = 0;
	appletGetMessage(&msg);
	switch(msg) {
        case 20:    //HomeButton
        {
            Power::Shutdown();
            break;
        }
        case 22:    //PowerButton
        {
            
            break;
        }
	}*/
}