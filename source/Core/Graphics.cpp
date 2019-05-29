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
u32 Graphics::defaultSelCol;
SDL_Rect Graphics::Screen;
Context Graphics::ctxt;

void Graphics::Init(SDL_Rect pos) {
    //Basic SDL init
    Screen = pos;
    SDL_Init(SDL_INIT_VIDEO);
    Window = SDL_CreateWindow("", Screen.x, Screen.y, Screen.w, Screen.h, 0);
    Rend = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(Rend, SDL_BLENDMODE_BLEND);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    SDL_SetRenderDrawColor(Rend, 0xFF, 0xFF, 0xFF, 0xFF);
    Graphics::ctxt = Context();
    
    appletRequestForeground();
    //appletSetHandlesRequestToDisplay(true);
}

void Graphics::Exit() {
    appletSetHandlesRequestToDisplay(false);
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(Rend);
    SDL_DestroyWindow(Window);
    SDL_Quit();
}

void Graphics::ClearScreen() {
	Rectangle(Screen.x, Screen.y, Screen.w, Screen.h, 0xFFFFFFFF);
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

SDL_Texture *Graphics::CreateTexFromString(std::string file) {
    SDL_Surface *surf = IMG_Load(file.c_str());
    if(!surf) return NULL;
    return CreateTexFromSurf(surf);
}

SDL_Texture *Graphics::CreateTexFromSurf(SDL_Surface *surf) {
    return SDL_CreateTextureFromSurface(Rend, surf);
}

void Graphics::RenderTexture(SDL_Texture *tex, SDL_Rect pos, SDL_Rect *clip) {
	if(!tex) return;
    if( clip != NULL ) {
        pos.w = clip->w;
        pos.h = clip->h;
    }
    SDL_RenderCopy(Rend, tex, clip, &pos);
}

void Graphics::DrawText(u8 fntsize, u32 x, u32 y, std::string str, u32 col, u32 wrap) {
    ctxt.color = col;
    ctxt.sizeIndex = fntsize;
    fonts().draw(Rend, x, y, str, ctxt);
}

void Graphics::DrawButton(SDL_Rect Pos, std::string Text, ButtonType butType) {
    int tHight = Fonts::FONT_SIZE_SMALL;
    switch(butType) {
        case BTN_Unselected:
        {
            Graphics::DrawText(Fonts::FONT_SMALL, Pos.x + 12, Pos.y + (Pos.h/2) - (tHight/2), Text);
            break;
        }
        case BTN_Selected_Lay1:
        {
            SDL_Rect pos; 
            pos.x = Pos.x-5; pos.y = Pos.y-5;
            pos.w = Pos.w+10; pos.h = Pos.h+10;
            Graphics::Rectangle(pos, GetDefaultSelCol());
            Graphics::Rectangle(Pos, GetDefaultButCol());
            Graphics::DrawText(Fonts::FONT_SMALL, Pos.x + 12, Pos.y + (Pos.h/2) - (tHight/2), Text, GetDefaultSelCol());
            break;
        }
        case BTN_Selected_Lay2:
        {
            Graphics::DrawText(Fonts::FONT_SMALL, Pos.x + 12, Pos.y + (Pos.h/2) - (tHight/2), Text, GetDefaultSelCol());
            break;
        }
    }
}

void Graphics::DrawOption(SDL_Rect Pos, std::string Text, std::string OptionText, bool isSel) {
    int tHight = Fonts::FONT_SIZE_SMALL;
    if(isSel) {
        SDL_Rect pos; 
        pos.x = Pos.x-5; pos.y = Pos.y-5;
        pos.w = Pos.w+10; pos.h = Pos.h+10;
        Graphics::Rectangle(pos, GetDefaultSelCol());
        Graphics::Rectangle(Pos, GetDefaultButCol());
        Graphics::DrawText(Fonts::FONT_SMALL, Pos.x + 10, Pos.y + (Pos.h/2) - (tHight/2), Text);
        Graphics::DrawText(Fonts::FONT_SMALL, Pos.x + ((Text != "") ? 200 : 40), Pos.y + (Pos.h/2) - (tHight/2), OptionText, GetDefaultSelCol());
    }
    else {
        Graphics::DrawText(Fonts::FONT_SMALL, Pos.x + 10, Pos.y + (Pos.h/2) - (tHight/2), Text);
        Graphics::DrawText(Fonts::FONT_SMALL, Pos.x + ((Text != "") ? 200 : 40), Pos.y + (Pos.h/2) - (tHight/2), OptionText);
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