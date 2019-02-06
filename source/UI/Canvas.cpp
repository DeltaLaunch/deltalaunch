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
    baseThemeDir = "/Theme/";
	INIReader cfg(baseThemeDir + "theme.cfg");
    
    //Setup bgs
    bgLay0 = cfg.Get("Background", "layer0", "None");
	bgLay1 = cfg.Get("Background", "layer1", "None");
	bgLay2 = cfg.Get("Background", "layer2", "None");
    
    //Setup fonts
    std::string font = cfg.Get("Config", "font", "");
    fntLarge = TTF_OpenFont((baseThemeDir + font).c_str(), cfg.GetInteger("Config", "bigFont", 25));
    fntMedium = TTF_OpenFont((baseThemeDir + font).c_str(), cfg.GetInteger("Config", "medFont", 20));
    fntSmall = TTF_OpenFont((baseThemeDir + font).c_str(), cfg.GetInteger("Config", "smallFont", 14));
    
    dash = Dashboard();
    
    //Create buttons to add to dash
    //dash.AddButton(Button(200, 700, 50, 50, nullptr));
    //dash.AddButton(Button(baseThemeDir + cfg.Get("NewsButton", "sprite", ""), cfg.GetInteger("NewsButton", "x", 200), cfg.GetInteger("NewsButton", "y", 700), nullptr));
    //dash.AddButton(Button(baseThemeDir + cfg.Get("ShopButton", "sprite", ""), cfg.GetInteger("ShopButton", "x", 350), cfg.GetInteger("ShopButton", "y", 700), nullptr));
    //dash.AddButton(Button(baseThemeDir + cfg.Get("AlbumButton", "sprite", ""), cfg.GetInteger("AlbumButton", "x", 500), cfg.GetInteger("AlbumButton", "y", 700), nullptr));
    
    //Get list of games to add to dash
    //App::GetList();
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

void Canvas::Render() {
    SDL_RenderPresent(mRender._renderer);
}

void Canvas::Clear() {
    SDL_RenderClear(mRender._renderer);
}

void Canvas::Update() {
	Hid::Check();

    // 1) Draw wallpaper
    dash.DrawWallpaper(bgLay0, bgLay1, bgLay2, baseThemeDir, mRender);
    
    // 2) Draw overlay
    dash.DrawButtons(mRender);
    Draw::Texture("/Theme/Graphics/Icons/News.png", 1280, 720, mRender);
    //Random debug text
    Draw::Text(fntSmall, mRender, 0, 0, "DeltaLaunch alpha!");
}