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

#include "Network.hpp"

CURLcode Network::Request(std::string url, RequestMethod method, std::string postdata) {
    CURL *curl = curl_easy_init();
    CURLcode res;
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        if(method == HTTP_POST)
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return res;
}