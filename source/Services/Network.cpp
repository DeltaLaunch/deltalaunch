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

Network* Network::instance = NULL;
static std::string readBuffer;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    readBuffer.append((char*)contents, realsize);
    return realsize;
}
        
std::string Network::Request(std::string url, RequestMethod method, std::string postdata) {
    CURL *curl = curl_easy_init();
    CURLcode res;
    long httpCode = 0;
    std::string outStr;
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        if(method == HTTP_POST)
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());
        res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        curl_easy_cleanup(curl);
        
        if(res != CURLE_OK) return "Curl error.";
        if(httpCode != 200) return std::string("Error: " + httpCode);
        else {
            outStr = readBuffer;
        }
    }
    return outStr;
}