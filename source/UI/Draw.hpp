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

#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <switch.h>
#include <stdint.h>
#include <string>
#include "Renderer.hpp"

class Draw
{
    public:
        static void Rectangle(u32 x, u32 y, u32 w, u32 h, u32 scolor, Renderer *rend);
        static void Rectangle(SDL_Rect pos, u32 scolor, Renderer *rend);
        static SDL_Texture *CreateTexFromSurf(SDL_Surface *surf, Renderer *rend);
        static void Text(Renderer *rend, TTF_Font *font, u32 x, u32 y, std::string str);
        static void DrawTexture(std::string tex, u32 x, u32 y, Renderer *rend);
        static void RenderTexture(SDL_Texture *tex, SDL_Rect pos, Renderer *rend);
};