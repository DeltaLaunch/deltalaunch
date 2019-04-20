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
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <switch.h>
#include <stdint.h>
#include <string>
#include "../Types.h"

#define GREY 0x4C4C4CFF

class Graphics
{
    public:
        static SDL_Renderer *Rend;
        static SDL_Window *Window;
        static TTF_Font *debugFont;
		static TTF_Font *hdrFont;
		static TTF_Font *smallFont;
        static void Init(std::string name, u32 width, u32 height, std::string font);
        static void Exit();
		static void ClearScreen();
        static void Rectangle(u32 x, u32 y, u32 w, u32 h, u32 scolor);
        static void Rectangle(SDL_Rect pos, u32 scolor);
        static SDL_Texture *CreateTexFromSurf(SDL_Surface *surf);
        static void DrawText(FontSize fntsize, u32 x, u32 y, std::string str);
        static void DrawText(FontSize fntsize, u32 x, u32 y, std::string str, u32 col);
        static void DrawTexture(std::string tex, u32 x, u32 y);
        static void RenderTexture(SDL_Texture *tex, SDL_Rect pos);
        static SDL_Renderer *GetRenderer() { return Rend; }
        static SDL_Surface *BufToSurf(void *buf, size_t size);
        static void DrawButton(SDL_Rect Pos, std::string Text, ButtonType butType);
        static void DrawOption(SDL_Rect Pos, std::string Text, std::string OptionText, bool isSel);
        static u32 GetDefaultSelCol() { return defaultSelCol; }
        static void SetDefaultSelCol(u32 col) { defaultSelCol = col; }
        static u32 GetDefaultButCol() { return GREY; }
        static void Render();
        static void Clear();
    private:
        static u32 defaultSelCol;
};