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

#include "Time.hpp"

std::string Time::GetClock(ClockType type) {
    u64 time = 0;
    TimeCalendarTime calTime;
    TimeCalendarAdditionalInfo info;
    #ifdef __SWITCH__
    timeGetCurrentTime(TimeType_Default, &time);
    timeToCalendarTimeWithMyRule(time, &calTime, &info);
    #else
    calTime.hour = 0;
    calTime.minute = 0;
    #endif
    char buff[10];
    std::string ampm[] = {"AM", "PM"};
    snprintf(buff, sizeof(buff), "%02d:%02d %s", type == CLOCK_12HR ? calTime.hour % 12 : calTime.hour, calTime.minute, (type == CLOCK_12HR) ? ampm[calTime.hour > 12 ? 1 : 0].c_str() : "");
    return std::string(buff);
}

std::string Time::GetDate() {
    u64 time = 0;
    TimeCalendarTime calTime;
    TimeCalendarAdditionalInfo info;
    std::string months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    #ifdef __SWITCH__
    timeGetCurrentTime(TimeType_Default, &time);
    timeToCalendarTimeWithMyRule(time, &calTime, &info);
    #else
    calTime.month = 0;
    calTime.year = 2019;
    #endif
    char buff[100];
    snprintf(buff, sizeof(buff), "%s %d, %d", months[calTime.month-1].c_str(), calTime.day, calTime.year);
    return std::string(buff);
}