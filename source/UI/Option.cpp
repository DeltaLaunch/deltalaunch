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

#include "Option.hpp"

Option::Option(std::string optName, const char** ops, u32 x, u32 y, u32 w, u32 h, u32 col, std::function<void()> callback) {
    Pos.x = x;
    Pos.y = y;
    Pos.w = w;
    Pos.h = h;
    Callback = callback;
    std::vector<std::string> vec (ops, ops + sizeof(ops) / sizeof(ops[0]) );
    Opts = vec;
    optIndex = 0;
    Text = optName;
}

Option::~Option() {
    //
}