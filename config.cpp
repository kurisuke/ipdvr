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

#include <algorithm>
#include <fstream>
#include "json/json.hpp"
#include <map>
#include <sstream>

using json = nlohmann::json;

Config::Config(std::string path) :
    m_path(path)
{
}

Config::~Config()
{
}

ChannelData::StdList parseChannels(const json& channelsroot)
{
    ChannelData::StdList ret;

    static const std::map<const std::string, ChannelData::ListingType> listingTypeMap =
    {
        {"json.xmltv.se", ChannelData::ListingType::JsonXmltvSe}
    };

    for (const auto& channel : channelsroot)
    {
        if(channel.is_object())
        {
            try
            {
                const auto name = channel.at("name").get<std::string>();
                const auto streamUrl = channel.at("streamUrl").get<std::string>();
                const auto listingTypeStr = channel.at("listingType").get<std::string>();
                const auto listingName = channel.at("listingName").get<std::string>();

                const auto listingType = listingTypeMap.at(listingTypeStr);

                ret.push_back(ChannelData(name, streamUrl, listingType, listingName));
                DEBUG_PRINT("Added new channel entry: " << name << std::endl);

            }
            catch(std::out_of_range&)
            {
                ERROR_PRINT("Found invalid channel entry, skipping..." << std::endl);
            }
            catch(std::domain_error&)
            {
                ERROR_PRINT("Found invalid channel entry, skipping..." << std::endl);
            }
        }
    }

    return ret;
}

bool Config::parse()
{
    std::ifstream ifs(m_path);

    // File exists?
    if (ifs.good())
    {
        const auto jsonDoc = json::parse(ifs);

        if (jsonDoc.is_object() && (jsonDoc.find("ipdvrConfig") != jsonDoc.end()))
        {
            DEBUG_PRINT("Found a valid ipdvrConfig root tag in file: " << m_path << std::endl);
        }
        else
        {
            ERROR_PRINT("Error parsing config file: " << m_path << std::endl);
            return false;
        }

        const auto rootNode = jsonDoc["ipdvrConfig"];
        if (rootNode.find("channels") != rootNode.end())
        {
            m_channelDataList = parseChannels(rootNode["channels"]);
            DEBUG_PRINT("Parsed a channel list with " << m_channelDataList.size() << " entries." << std::endl);
            return true;
        }
        else
        {
            ERROR_PRINT("No channels defined!" << std::endl);
            return false;
        }
    }
    else
    {
        DEBUG_PRINT("Could not open file: " << m_path << std::endl);
        return false;
    }
}

ChannelData Config::getChannelData(const std::string& name)
{
    auto it = std::find_if(m_channelDataList.cbegin(), m_channelDataList.cend(),
                           [name](ChannelData c) {return c.name == name; });
    if(it != m_channelDataList.cend())
    {
        return (*it);
    }
    else
    {
        return ChannelData(); // invalid default object
    }
}

const ChannelData::StdList& Config::getChannelData()
{
    return m_channelDataList;
}
