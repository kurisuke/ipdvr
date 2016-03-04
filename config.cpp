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

#include "config.h"

#include "debug.h"

#include <fstream>
#include "rapidjson/document.h"
#include <sstream>

Config::Config(std::string path) :
    m_path(path)
{
}

Config::~Config()
{
}

bool Config::parse()
{
    std::ifstream ifs(m_path);

    // File exists?
    if (ifs.good())
    {
        // Read file into stringstream
        std::stringstream buffer;
        buffer << ifs.rdbuf();

        // convert to c char and parse
        rapidjson::Document jsonDoc;
        jsonDoc.Parse(buffer.str().c_str());

        if (jsonDoc.IsObject() && jsonDoc.HasMember("ipdvrConfig"))
        {
            DEBUG_PRINT("Parsed a valid config file: " << m_path << std::endl);
            // TODO: save content to Config members
            return true;
        }
        else
        {
            DEBUG_PRINT("Error parsing config file: " << m_path << std::endl);
            return false;
        }

    }
    else
    {
        DEBUG_PRINT("Could not open file: " << m_path << std::endl);
        return false;
    }
}
