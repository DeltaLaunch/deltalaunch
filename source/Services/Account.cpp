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

#include "Account.hpp"

s32 Account::GetUserCount() {
    s32 cnt = 0;
    #ifdef __SWITCH__
    accountInitialize();
    accountGetUserCount(&cnt);
    accountExit();
    #endif
    return cnt;
}

u128 Account::GetActiveAccount() {
	u128 userID;
    bool sel;
    #ifdef __SWITCH__
	accountInitialize();
	Result rc = accountGetActiveUser(&userID, &sel);
	accountExit();
    if(R_FAILED(rc) || !userID || !sel)
        userID = App::LaunchPSelect();
    #endif
	return userID;
}

#define MAX_USERS 8
u128 Account::GetFirstAccount() {
    u128 userIDs[MAX_USERS];
    #ifdef __SWITCH__
    size_t total = 0;
    accountInitialize();
    accountListAllUsers(userIDs, MAX_USERS, &total);
    accountExit();

    if(!total){
        // do error logging
        return (u128)-1;
    }
    #endif
    return userIDs[0];
}

Result Account::SetCustomProfileImage(std::string filename) {
    FILE *fp = fopen(filename.c_str(), "rb");
    Result rc = -1;
    if(fp){
        u128 uid = Account::GetFirstAccount();
        size_t fsize = 0;
        fseek(fp, 0, SEEK_END);
        fsize = ftell(fp);
        rewind(fp);
        u8 buf[fsize] = {0};
        fread(buf, fsize, 1, fp);
        fclose(fp);
        AccountProfile acc;
        AccountProfileBase pb;
        AccountUserData ud;
        accountInitialize();
        rc = accountGetProfile(&acc, uid);
        rc = accountProfileGet(&acc, &ud, &pb);
        ud.iconID = 0;
        rc = accGetProfileEditor(&acc, uid);
        rc = accStoreWithImage(&acc, &pb, &ud, buf, fsize);
        accountProfileClose(&acc);
        accountExit();
    }
    return rc;
}

u128 Account::TryGetUser() {
    u128 userID;
    #ifdef __SWITCH__
    accountInitialize();
    Result rc = accTrySelectUserWithoutInteraction(&userID);
    accountExit();
    if(R_FAILED(rc) || !userID)
        userID = App::LaunchPSelect();
    #endif
    return userID;
}

SDL_Texture *Account::GetProfileImage(u128 userID) {
    SDL_Texture *tex = NULL;
    #ifdef __SWITCH__
    if(userID) {
        AccountProfile acc;
        size_t imgSize = 0;
        accountInitialize();
        accountGetProfile(&acc, userID);
        accountProfileGetImageSize(&acc, &imgSize);
        void *buf = malloc(imgSize);
        accountProfileLoadImage(&acc, buf, imgSize, &imgSize);
        SDL_Surface *img = Graphics::BufToSurf(buf, imgSize);
        tex = Graphics::CreateTexFromSurf(img);
        free(buf);
        accountProfileClose(&acc);
        accountExit();
    }
    #endif
    return tex;
}

Result Account::SetProfilePicture(u128 userId, u8 *jpg, size_t jpgSize) {
	Result rc = 0;
    #ifdef __SWITCH__
	AccountProfile prof;
	AccountProfileBase pb;
	AccountUserData accData;
	rc = accGetProfileEditor(&prof, userId);
	rc = accStoreWithImage(&prof, &pb, &accData, jpg, jpgSize);
	#endif
	return rc;
}