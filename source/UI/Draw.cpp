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

void Draw::Rectangle(int x, int y, int w, int h, SDL_Color scolor, Renderer rend) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(rend._renderer, scolor.r, scolor.g, scolor.b, scolor.a);
    SDL_RenderFillRect(rend._renderer, &rect);
}

void Draw::Texture(std::string tex, uint8_t x, uint8_t y, Renderer rend) {
	SDL_Surface *bgs = IMG_Load(tex.c_str());
    SDL_Texture *bgt = SDL_CreateTextureFromSurface(rend._renderer, bgs);
    SDL_Rect position = {x, y, bgs->w, bgs->h};
    SDL_FreeSurface(bgs);
    
    SDL_RenderCopy(rend._renderer, bgt, NULL, &position);
    SDL_DestroyTexture(bgt);
}

void Draw::Text(TTF_Font *font, Renderer rend, int x, int y, std::string str) {
    SDL_Color scolor = {0xFF, 0xFF, 0xFF, 0xFF};
    SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(font, str.c_str(), scolor, 1280);

	if (!surface) {
		return;
	}

	SDL_SetSurfaceAlphaMod(surface, 255);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(rend._renderer, surface);

	if (texture) {
		SDL_Rect position;
		position.x = x;
		position.y = y;
		position.w = surface->w;
		position.h = surface->h;

		SDL_RenderCopy(rend._renderer, texture, NULL, &position);
		SDL_DestroyTexture(texture);
	}
	SDL_FreeSurface(surface);
}