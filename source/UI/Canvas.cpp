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

Canvas::Canvas(uint32_t width, uint32_t height) {
    //Read config file
    baseThemeDir = "/Theme/";
	INIReader cfg(baseThemeDir + "theme.cfg");
    
    //Basic SDL init
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(width, height, 0, &mRender._window, &mRender._renderer);
    mRender._surface = SDL_GetWindowSurface(mRender._window);
    SDL_SetRenderDrawBlendMode(mRender._renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    SDL_SetRenderDrawColor(mRender._renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    Mix_VolumeMusic(cfg.GetInteger("Background", "bgmVol", 64));
    
    //Setup background
    bgm = Mix_LoadMUS(cfg.Get("Background", "bgm", "").c_str());
    bgLay0 = cfg.Get("Background", "layer0", "");
	bgLay1 = cfg.Get("Background", "layer1", "");
	bgLay2 = cfg.Get("Background", "layer2", "");
    
    //Setup fonts
    std::string font = cfg.Get("Config", "font", "");
    fntLarge = TTF_OpenFont((baseThemeDir + font).c_str(), cfg.GetInteger("Config", "bigFont", 25));
    fntMedium = TTF_OpenFont((baseThemeDir + font).c_str(), cfg.GetInteger("Config", "medFont", 20));
    fntSmall = TTF_OpenFont((baseThemeDir + font).c_str(), cfg.GetInteger("Config", "smallFont", 14));
    
    dash = Dashboard();
    
    //Create buttons to add to dash
    dash.AddButton(Button(baseThemeDir + cfg.Get("NewsButton", "sprite", ""), cfg.GetInteger("NewsButton", "x", 250), cfg.GetInteger("NewsButton", "y", 600), nullptr));
    dash.AddButton(Button(baseThemeDir + cfg.Get("ShopButton", "sprite", ""), cfg.GetInteger("ShopButton", "x", 400), cfg.GetInteger("ShopButton", "y", 600), nullptr));
    dash.AddButton(Button(baseThemeDir + cfg.Get("AlbumButton", "sprite", ""), cfg.GetInteger("AlbumButton", "x", 550), cfg.GetInteger("AlbumButton", "y", 600), nullptr));
    dash.AddButton(Button(baseThemeDir + cfg.Get("HomebrewButton", "sprite", ""), cfg.GetInteger("HomebrewButton", "x", 700), cfg.GetInteger("HomebrewButton", "y", 600), nullptr));
    dash.AddButton(Button(baseThemeDir + cfg.Get("SettingsButton", "sprite", ""), cfg.GetInteger("SettingsButton", "x", 850), cfg.GetInteger("SettingsButton", "y", 600), nullptr));
    dash.AddButton(Button(baseThemeDir + cfg.Get("PowerButton", "sprite", ""), cfg.GetInteger("PowerButton", "x", 1000), cfg.GetInteger("PowerButton", "y", 600), Power::Shutdown));
	
	Menus.push_back(Menu("Test"));
    
    //Play BGM
    if(bgm) Mix_PlayMusic(bgm, -1);
}

Canvas::~Canvas() {
    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_DestroyRenderer(mRender._renderer);
    SDL_FreeSurface(mRender._surface);
    SDL_DestroyWindow(mRender._window);
    SDL_Quit();
}

void Canvas::Render() {
    SDL_RenderPresent(mRender._renderer);
}

void Canvas::Clear() {
    SDL_RenderClear(mRender._renderer);
}

void Canvas::Update() {
	Hid::Check();
	dash.Update();
	
    // 1) Draw wallpaper
    dash.DrawWallpaper(bgLay0, bgLay1, bgLay2, baseThemeDir, mRender);
    
    // 2) Draw overlay
    dash.DrawButtons(mRender);
    
    //Random debug text
	Debug dbg(fntSmall);
	touchPosition touchPos;
	hidTouchRead(&touchPos, 0);
    dbg.Print(mRender, "DeltaLaunch alpha!");
    dbg.Print(mRender, "Touch: X=" + std::to_string(touchPos.px) + "; y=" + std::to_string(touchPos.py));
}