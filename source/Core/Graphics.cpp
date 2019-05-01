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

#include "Graphics.hpp"

SDL_Renderer *Graphics::Rend;
SDL_Window *Graphics::Window;
TTF_Font *Graphics::debugFont;
TTF_Font *Graphics::hdrFont;
TTF_Font *Graphics::smallFont;
u32 Graphics::defaultSelCol;
u32 Graphics::winWidth;
u32 Graphics::winHeight;

void Graphics::Init(std::string name, u32 width, u32 height, std::string font) {
    //Basic SDL init
    SDL_Init(SDL_INIT_EVERYTHING);
    Window = SDL_CreateWindow(name.c_str(), 0, 0, width, height, 0);
    Rend = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(Rend, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    SDL_SetRenderDrawColor(Rend, 0xFF, 0xFF, 0xFF, 0xFF);
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
    Graphics::winWidth = width;
    Graphics::winHeight = height;
    
    plInitialize();
    if(font == "") {
        PlFontData fnt;
        plGetSharedFontByType(&fnt, PlSharedFontType_Standard);
        debugFont = TTF_OpenFontRW(SDL_RWFromMem(fnt.address, fnt.size), 1, 14);
        hdrFont = TTF_OpenFontRW(SDL_RWFromMem(fnt.address, fnt.size), 1, 28);
        smallFont = TTF_OpenFontRW(SDL_RWFromMem(fnt.address, fnt.size), 1, 20);
    } 
    else {
        debugFont = TTF_OpenFont(font.c_str(), 14);
        hdrFont = TTF_OpenFont(font.c_str(), 28);
        smallFont = TTF_OpenFont(font.c_str(), 20);
    }
    
    appletRequestForeground();
    //appletSetHandlesRequestToDisplay(true);
}

void Graphics::Exit() {
    appletSetHandlesRequestToDisplay(false);
    TTF_CloseFont(debugFont);
	TTF_CloseFont(hdrFont);
	TTF_CloseFont(smallFont);
    plExit();
    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_DestroyRenderer(Rend);
    SDL_DestroyWindow(Window);
    SDL_Quit();
}

void Graphics::ClearScreen() {
	Rectangle(0, 0, winWidth, winHeight, 0xFFFFFFFF);
}

void Graphics::BorderedRectangle(SDL_Rect pos, u32 foreCol, u32 bordCol, u8 bordThick) {
    SDL_Rect p = pos;
    p.x -= bordThick;
    p.y -= bordThick;
    p.w += (bordThick*2);
    p.h += (bordThick*2);
    Rectangle(p, bordCol);
    Rectangle(pos, foreCol);
}

void Graphics::Rectangle(u32 x, u32 y, u32 w, u32 h, u32 scolor) {
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    pos.w = w;
    pos.h = h;
    
    Rectangle(pos, scolor);
}

void Graphics::Rectangle(SDL_Rect pos, u32 scolor) {
    SDL_SetRenderDrawColor(Rend, ((scolor >> 24) & 0xFF), ((scolor >> 16) & 0xFF), ((scolor >> 8) & 0xFF), (scolor & 0xFF));
    SDL_RenderFillRect(Rend, &pos);
}

void Graphics::DrawTexture(std::string tex, u32 x, u32 y) {
    SDL_Surface *bgs = IMG_Load(tex.c_str()); //Maybe cache images in the future so not to constantly read SD?
    if(!bgs) return;
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    pos.w = bgs->w;
    pos.h = bgs->h;
    SDL_Texture *t = SDL_CreateTextureFromSurface(Rend, bgs);
	if(t) {
		RenderTexture(t, pos);
		SDL_FreeSurface(bgs);
	}
    SDL_DestroyTexture(t);
}

SDL_Texture *Graphics::CreateTexFromSurf(SDL_Surface *surf) {
    return SDL_CreateTextureFromSurface(Rend, surf);
}

void Graphics::RenderTexture(SDL_Texture *tex, SDL_Rect pos) {
	if(!tex) return;
    SDL_RenderCopy(Rend, tex, NULL, &pos);
}

void Graphics::DrawText(FontSize fntsize, u32 x, u32 y, std::string str) {
    DrawText(fntsize, x, y, str, 0xFFFFFFFF, winWidth);
}

void Graphics::DrawText(FontSize fntsize, u32 x, u32 y, std::string str, u32 col) {
    DrawText(fntsize, x, y, str, col, winWidth);
}

void Graphics::DrawText(FontSize fntsize, u32 x, u32 y, std::string str, u32 col, u32 wrap) {
    SDL_Color scolor;
    scolor.r = (col >> 24)&0xFF; scolor.g = (col >> 16)&0xFF; scolor.b = (col >> 8)&0xFF; scolor.a = col&0xFF;
    SDL_Surface *surface;
    
    switch(fntsize) {
        case FNT_Small:
            surface = TTF_RenderUTF8_Blended_Wrapped(smallFont, str.c_str(), scolor, wrap);
            break;
        case FNT_Big:
            surface = TTF_RenderUTF8_Blended_Wrapped(hdrFont, str.c_str(), scolor, wrap);
            break;
        case FNT_Debug:
            surface = TTF_RenderUTF8_Blended_Wrapped(debugFont, str.c_str(), scolor, wrap);
            break;
    }
    if (!surface) return;

    SDL_SetSurfaceAlphaMod(surface, 0xFF);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Rend, surface);

    if (texture) {
        SDL_Rect pos;
        pos.x = x;
        pos.y = y;
        pos.w = surface->w;
        pos.h = surface->h;
        SDL_RenderCopy(Rend, texture, NULL, &pos);
        SDL_DestroyTexture(texture);
    }
    SDL_FreeSurface(surface);
}

void Graphics::DrawButton(SDL_Rect Pos, std::string Text, ButtonType butType) {
    int tHight = 0;
    TTF_SizeText(smallFont, Text.c_str(), NULL, &tHight);
    switch(butType) {
        case BTN_Unselected:
        {
            Graphics::DrawText(FNT_Small, Pos.x + 12, Pos.y + (Pos.h/2) - (tHight/2), Text);
            break;
        }
        case BTN_Selected_Lay1:
        {
            
            SDL_Rect pos; 
            pos.x = Pos.x-5; pos.y = Pos.y-5;
            pos.w = Pos.w+10; pos.h = Pos.h+10;
            Graphics::Rectangle(pos, GetDefaultSelCol());
            Graphics::Rectangle(Pos, GetDefaultButCol());
            Graphics::DrawText(FNT_Small, Pos.x + 12, Pos.y + (Pos.h/2) - (tHight/2), Text, GetDefaultSelCol());
            break;
        }
        case BTN_Selected_Lay2:
        {
            Graphics::DrawText(FNT_Small, Pos.x + 12, Pos.y + (Pos.h/2) - (tHight/2), Text, GetDefaultSelCol());
            break;
        }
    }
}

void Graphics::DrawOption(SDL_Rect Pos, std::string Text, std::string OptionText, bool isSel) {
    int tHight = 0, tWid = 0;
    TTF_SizeText(smallFont, Text.c_str(), &tWid, &tHight);
    if(isSel) {
        SDL_Rect pos; 
        pos.x = Pos.x-5; pos.y = Pos.y-5;
        pos.w = Pos.w+10; pos.h = Pos.h+10;
        Graphics::Rectangle(pos, GetDefaultSelCol());
        Graphics::Rectangle(Pos, GetDefaultButCol());
        Graphics::DrawText(FNT_Small, Pos.x + 10, Pos.y + (Pos.h/2) - (tHight/2), Text);
        Graphics::DrawText(FNT_Small, Pos.x + ((Text != "") ? 200 : 40), Pos.y + (Pos.h/2) - (tHight/2), OptionText, GetDefaultSelCol());
    }
    else {
        Graphics::DrawText(FNT_Small, Pos.x + 10, Pos.y + (Pos.h/2) - (tHight/2), Text);
        Graphics::DrawText(FNT_Small, Pos.x + ((Text != "") ? 200 : 40), Pos.y + (Pos.h/2) - (tHight/2), OptionText);
    }
}

SDL_Surface *Graphics::BufToSurf(void *buf, size_t size) {
    return IMG_Load_RW(SDL_RWFromMem(buf, size), 1);
}

void Graphics::Render() {
    SDL_RenderPresent(Rend);
}

void Graphics::Clear() {
    SDL_RenderClear(Rend);
}