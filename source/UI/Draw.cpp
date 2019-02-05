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

#include "Draw.hpp"

void Draw::Rectangle(int x, int y, int w, int h, SDL_Color scolor, Render rend) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_SetRenderDrawColor(rend._renderer, scolor.r, scolor.g, scolor.b, scolor.a);
    SDL_RenderFillRect(rend._renderer, &rect);
}

void Draw::Text(TTF_Font *font, Render rend, int x, int y, std::string str) {
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