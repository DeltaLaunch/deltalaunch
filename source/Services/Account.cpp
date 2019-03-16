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
    accountInitialize();
    accountGetUserCount(&cnt);
    accountExit();
    return cnt;
}

u128 Account::GetFirstAccount() {
	u128 *userIDs = (u128*)malloc(8*sizeof(u128));
	size_t total;
	accountInitialize();
	accountListAllUsers(userIDs, 8, &total);
	accountExit();
	u128 id = userIDs[0];
	free(userIDs);
	return id;
}

Result Account::SetProfilePicture(u128 userId, u8 *jpg, size_t jpgSize) {
	Result rc = 0;
	AccountProfile prof;
	AccountProfileBase pb;
	AccountUserData accData;
	
	rc = accGetProfileEditor(&prof, userId);
	rc = accStoreWithImage(&prof, &pb, &accData, jpg, jpgSize);
	
	return rc;
}