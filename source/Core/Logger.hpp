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

class Logger {
    public:
        static void write(std::string str) {
            FILE *fp = fopen("/log.txt", "a+");
            std::string s = (std::string("[Log] ") + str + std::string("\n")).c_str();
            fwrite(s.c_str(), strlen(s.c_str()), 1, fp);
            fclose(fp);
        }
};