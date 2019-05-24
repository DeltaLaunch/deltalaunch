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

Panel::Panel(u32 x, u32 y){
    Pos.x = x;
    Pos.y = y;
    optSelect = 0;
}

Panel::~Panel() {
    for(auto el: Elements) delete el;
    Elements.clear();
    Strings.clear();
}

void Panel::IncrementSelect() {
    if(optSelect < Elements.size()-1)
        optSelect++;
    else
        optSelect = Elements.size()-1;
}

void Panel::DecrementSelect() {
    if(optSelect > 0)
        optSelect--;
    else
        optSelect = 0;
}

void Panel::Update(bool selected) {
    unsigned ind = 0;
    
    for(auto str: Strings) {
        Graphics::DrawText(Fonts::FONT_SMALL, std::get<0>(Strings.at(ind))+Pos.x, std::get<1>(Strings.at(ind))+Pos.y, std::get<2>(Strings.at(ind)));
        ind++;
    }
    
    ind = 0;
    for(auto elem: Elements) {
        elem->Draw(Pos, (ind == optSelect) && selected);
        ind++;
    }
    if(selected){        
        if(Hid::Input & KEY_DUP) DecrementSelect();
        if(Hid::Input & KEY_DDOWN) IncrementSelect();
        if(Hid::Input & KEY_A) {
            if(Elements[optSelect]->HasFunc())
                Elements[optSelect]->Run();
        }
    }
}