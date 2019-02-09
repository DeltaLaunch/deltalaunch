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

#include "Draw.hpp"

void Draw::Rectangle(int x, int y, int w, int h, u32 scolor, Renderer rend) {
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    pos.w = w;
    pos.h = h;
    
    SDL_SetRenderDrawColor(rend._renderer, ((scolor >> 24) & 0xFF), ((scolor >> 16) & 0xFF), ((scolor >> 8) & 0xFF), (scolor & 0xFF));
    SDL_RenderFillRect(rend._renderer, &pos);
}

void Draw::Texture(std::string tex, u32 x, u32 y, Renderer rend) {
    SDL_Surface *bgs = IMG_Load(tex.c_str()); //Maybe cache images in the future so not to constantly read SD?
    if(!bgs) return;
    SDL_Texture *bgt = SDL_CreateTextureFromSurface(rend._renderer, bgs);
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    pos.w = bgs->w;
    pos.h = bgs->h;
    SDL_RenderCopy(rend._renderer, bgt, NULL, &pos);
    SDL_FreeSurface(bgs);
    SDL_DestroyTexture(bgt);
}

void Draw::Text(TTF_Font *font, Renderer rend, int x, int y, std::string str) {
    SDL_Color scolor;
    scolor.r = scolor.g = scolor.b = scolor.a = 0xFF;
    SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(font, str.c_str(), scolor, 1280);
    if (!surface) return;

    SDL_SetSurfaceAlphaMod(surface, 0xFF);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(rend._renderer, surface);

    if (texture) {
        SDL_Rect pos;
        pos.x = x;
        pos.y = y;
        pos.w = surface->w;
        pos.h = surface->h;
        SDL_RenderCopy(rend._renderer, texture, NULL, &pos);
        SDL_DestroyTexture(texture);
    }
    SDL_FreeSurface(surface);
}