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

#include "Dashboard.hpp"

Dashboard::Dashboard(Renderer *rend, u32 width, u32 height, std::string font) {
    Rend = rend;
    Width = width;
    Height = height;
    Font = TTF_OpenFont(font.c_str(), 14);
    lastErr = 0;
    IsMenuOpen = false;
    debugInfo = false;
    Wallpaper = SDL_CreateTexture(Rend->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);
    LockScreen = SDL_CreateTexture(Rend->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);
}

Dashboard::~Dashboard() {
    TTF_CloseFont(Font);
	for(auto button: Buttons) {
        delete button;
    }
	for(auto menu: Menus) {
        delete menu;
    }
    Buttons.clear();
	Menus.clear();
    SDL_DestroyTexture(Wallpaper);
    SDL_DestroyTexture(LockScreen);
}

void Dashboard::DrawWallpaper() {
    SDL_Rect pos;
    pos.x = 0; pos.y = 0;
    pos.w = Width; pos.h = Height;
	Draw::RenderTexture(Wallpaper, pos, Rend);
}

void Dashboard::SetWallpaper(std::string lay0, std::string lay1, std::string lay2) {
    SDL_Surface *l0 = IMG_Load(lay0.c_str());
    SDL_Surface *l1 = IMG_Load(lay1.c_str());
    SDL_Surface *l2 = IMG_Load(lay2.c_str());
    SDL_Surface *wall = SDL_CreateRGBSurface(0,l0->w,l0->h,32,0,0,0,0);
    SDL_Rect pos;
    pos.x = 0; pos.y = 0;
    pos.w = l0->w; pos.h = l0->h;
    SDL_BlitSurface(l0, &pos, wall, NULL);
    SDL_BlitSurface(l1, &pos, wall, NULL);
    SDL_BlitSurface(l2, &pos, wall, NULL);
    Wallpaper = SDL_CreateTextureFromSurface(Rend->_renderer, wall);
    SDL_FreeSurface(l0);
    SDL_FreeSurface(l1);
    SDL_FreeSurface(l2);
    SDL_FreeSurface(wall);
}

void Dashboard::DrawLockScreen() {
    SDL_Rect pos;
    pos.x = 0; pos.y = 0;
    pos.w = Width; pos.h = Height;
	Draw::RenderTexture(LockScreen, pos, Rend);
}

void Dashboard::SetLockScreen(std::string image) {
    SDL_Surface *img = IMG_Load(image.c_str());
    LockScreen = SDL_CreateTextureFromSurface(Rend->_renderer, img);
    SDL_FreeSurface(img);
}

void Dashboard::DrawButtons() {
    for(auto button: Buttons) {
        if(button->Sprite != nullptr)
            Draw::RenderTexture(button->Sprite, button->Pos, Rend);
        else
            Draw::Rectangle(button->Pos, button->Color, Rend);
        
        if(Hid::IsTouched(button->Pos) && !IsMenuOpen) {
            lastErr = button->Run();
			if(lastErr) App::ShowError("An Error has occurred!", "Error code: " + std::to_string(lastErr), lastErr);
		}
    }
}

void Dashboard::DrawMenus() {
    for(auto menu: Menus) {
        if(menu->IsOpen()) {
            if(menu->Sprite != nullptr)
				Draw::RenderTexture(menu->Sprite, menu->Pos, Rend);
			else
				Draw::Rectangle(menu->Pos, menu->Color, Rend);
        }
    }
}

void Dashboard::DrawDebugText() {
    if(debugInfo && Font) {
        touchPosition touchPos;
        hidTouchRead(&touchPos, 0);
        u32 X = 14,  Y = 0, s = 14;
        Draw::Text(Rend, Font, X, Y+=s, "DeltaLaunch alpha!");
        Draw::Text(Rend, Font, X, Y+=s, "Firmware: " + Settings::GetFirmwareVersion());
        Draw::Text(Rend, Font, X, Y+=s, "Serial: " + Settings::GetSerialNumber());
        Draw::Text(Rend, Font, X, Y+=s, "Touch: X=" + std::to_string(touchPos.px) + "; y=" + std::to_string(touchPos.py));
        Y = 0;
    }
}

Result Dashboard::OpenMenu(std::string name) {
	for(auto menu: Menus) {
        if(menu->GetTitle() == name) {
			IsMenuOpen = true;;
			menu->Show();
		}
    }
    return 0;
}

Result Dashboard::CloseMenus() {
	for(auto menu: Menus) {
        if(menu->IsOpen()) {
            IsMenuOpen = false;
			menu->Hide();
		}
    }
    return 0;
}

void Dashboard::AddButton(Button *button) {
    Buttons.push_back(button);
}

void Dashboard::AddMenu(Menu *menu) {
    Menus.push_back(menu);
}