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

#include "Game.hpp"

Game::Game() {
    Sprite = nullptr;
    Icon = nullptr;
	TitleId = 0;
	Flag = 0;
    SelColor = AQUA;
}

Game::~Game() {
	SDL_DestroyTexture(Icon);
	SDL_DestroyTexture(Sprite);
}

void Game::MountSaveData() {
    FsSave save = {0};
    save.titleID = TitleId;
    FsFileSystem *fs = fsdevGetDefaultFileSystem();
    fsMountSaveData(fs, 0, &save);
}

Result Game::Play() {
    Result rc = 0;
    if(!App::currentApplication.active) {
        u128 userid = App::LaunchPSelect();
        if(userid == 0) return 0;
        rc = App::LaunchGame(TitleId, userid);
        rc = appletRequestForeground();
        //leet hax
        appletSetHandlesRequestToDisplay(false);
        appletSetHandlesRequestToDisplay(true);
    }
    else {
        rc = appRequestForApplicationToGetForeground(&App::currentApplication);
    }
    return rc;
}