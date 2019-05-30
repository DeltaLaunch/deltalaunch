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
#include <switch.h>
#include <string>
#include <curl/curl.h>

class Network
{
    public:
        enum RequestMethod {
            HTTP_GET,
            HTTP_POST
        };
        
        static Network* getInstance() {
            if (instance == nullptr)
                instance = new Network();
            return instance;
        };
        
        std::string Request(std::string url, RequestMethod method, std::string postdata = "");
        
    private:
        static Network* instance;
        
        Network() {
            curl_global_init(CURL_GLOBAL_ALL);
        };
        
        Network(const Network&);
        Network& operator=(const Network&);
        
};