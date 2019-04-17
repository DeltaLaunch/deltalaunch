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

#include "Panel.hpp"

Panel::Panel(TTF_Font *font, u32 x, u32 y){
    Pos.x = x;
    Pos.y = y;
    Font = font;
    optSelect = 0;
}

Panel::~Panel() {
    for(auto opts: Options) delete opts;
    for(auto pics: Images) SDL_DestroyTexture(std::get<4>(pics));
    Options.clear();
    Strings.clear();
    Images.clear();
}

void Panel::IncrementSelect() {
    if(optSelect < Options.size()-1)
        optSelect++;
    else
        optSelect = 0;
}

void Panel::DecrementSelect() {
    if(optSelect > 0)
        optSelect--;
    else
        optSelect = Options.size()-1;
}

void Panel::Update(u32 kDown, bool selected) {
    for(int s = 0; s < (int)Strings.size(); s++) {
        Graphics::DrawText(Font, std::get<0>(Strings.at(s))+Pos.x, std::get<1>(Strings.at(s))+Pos.y, std::get<2>(Strings.at(s)));
    }
    unsigned ind = 0;
    for(auto opt: Options) {
        SDL_Rect p = opt->Pos;
        p.x += Pos.x;
        p.y += Pos.y;
        Graphics::DrawOption(Font, p, opt->Text, opt->GetOptText(), (ind == optSelect) && selected);
        ind++;
    }
    for(int i = 0; i < (int)Images.size(); i++) {
        SDL_Rect p;
        p.x = Pos.x + std::get<0>(Images.at(i));
        p.y = Pos.y + std::get<1>(Images.at(i));
        p.w = std::get<2>(Images.at(i));
        p.h = std::get<3>(Images.at(i));
        Graphics::RenderTexture(std::get<4>(Images.at(i)), p);
        ind++;
    }
    if(selected){        
        if(kDown & KEY_DUP) IncrementSelect();
        if(kDown & KEY_DDOWN) DecrementSelect();
        if(kDown & KEY_A) {
            if(Options[optSelect]->HasFunc())
                Options[optSelect]->Run();
        }
    }
}