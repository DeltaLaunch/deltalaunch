/*
* Qlaunch
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

#include "Canvas.hpp"

void Canvas::Init() {
    //Basic SDL init
    romfsInit();
    fsdevMountSdmc();
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(1280, 720, 0, &mRender._window, &mRender._renderer);
    mRender._surface = SDL_GetWindowSurface(mRender._window);
    SDL_SetRenderDrawBlendMode(mRender._renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    SDL_SetRenderDrawColor(mRender._renderer, 255, 255, 255, 255);
    
    //Read config file
    INIReader cfg("/Theme/theme.cfg");
    baseThemeDir = "/Theme/";
    std::string font = cfg.Get("Config", "font", "UNK");
    BackgroundPath = cfg.Get("Config", "background", "UNK");
    
    //Setup fonts
    fntLarge = TTF_OpenFont((baseThemeDir + font).c_str(), 25);
    fntMedium = TTF_OpenFont((baseThemeDir + font).c_str(), 20);
    fntSmall = TTF_OpenFont((baseThemeDir + font).c_str(), 14);
}

void Canvas::SetTheme() {
    SDL_Surface *bgs = IMG_Load((baseThemeDir + BackgroundPath).c_str());
    if (bgs){
        Uint32 colorkey = SDL_MapRGB(bgs->format, 0, 0, 0);
        SDL_SetColorKey(bgs, SDL_TRUE, colorkey);
    }
    SDL_Texture *bgt = SDL_CreateTextureFromSurface(mRender._renderer, bgs);
    SDL_Rect position;
    position.x = 0;
    position.y = 0;
    position.w = bgs->w;
    position.h = bgs->h;
    SDL_RenderCopy(mRender._renderer, bgt, NULL, &position);
    SDL_DestroyTexture(bgt);
    SDL_FreeSurface(bgs);
}

void Canvas::Free() {
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(mRender._renderer);
    SDL_FreeSurface(mRender._surface);
    SDL_DestroyWindow(mRender._window);
    SDL_Quit();
    romfsExit();
    fsdevUnmountAll();
}