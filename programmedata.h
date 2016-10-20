/* ipdvr - DVR for IPTV streams
 *
 * Copyright (C) 2016  Peter Helbing
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef PROGRAMMEDATA_H
#define PROGRAMMEDATA_H

#include <chrono>
#include <list>
#include <string>

struct ProgrammeData
{
    typedef std::list<ProgrammeData> StdList;

    bool valid;
    std::string title;
    std::string desc;
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> stop;

    ProgrammeData(const std::string& c_title,
                  const std::string& c_desc,
                  int c_start,
                  int c_stop) :
        valid(true),
        title(c_title),
        desc(c_desc),
        start(std::chrono::time_point<std::chrono::system_clock>(std::chrono::seconds(c_start))),
        stop(std::chrono::time_point<std::chrono::system_clock>(std::chrono::seconds(c_stop)))
    {}


    ProgrammeData() :
        valid(false)
    {}
};

#endif // PROGRAMMEDATA_H
