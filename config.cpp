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
#include <map>
#include "rapidjson/document.h"
#include <sstream>

Config::Config(std::string path) :
    m_path(path)
{
}

Config::~Config()
{
}

ChannelData::StdList parseChannels(const rapidjson::Value& channelsroot)
{
    ChannelData::StdList ret;

    if (!channelsroot.IsArray())
    {
        ERROR_PRINT("Not a channels array, parsing failed.");
        return ret;
    }

    static const std::map<const std::string, ChannelData::ListingType> listingTypeMap =
    {
        {"json.xmltv.se", ChannelData::ListingType::JsonXmltvSe}
    };

    for (rapidjson::Value::ConstValueIterator it = channelsroot.Begin(); it != channelsroot.End(); ++it)
    {
        if(it->IsObject())
        {
            if ((it->HasMember("name")) &&
                (it->HasMember("streamUrl")) &&
                (it->HasMember("listingType")) &&
                (it->HasMember("listingName")))
            {
                const std::string listingTypeStr = (*it)["listingType"].GetString();

                if (listingTypeMap.count(listingTypeStr))
                {
                    const std::string name = (*it)["name"].GetString();
                    const std::string streamUrl = (*it)["streamUrl"].GetString();
                    const ChannelData::ListingType listingType = listingTypeMap.at(listingTypeStr);
                    const std::string listingName = (*it)["listingName"].GetString();

                    ret.push_back(ChannelData(name, streamUrl, listingType, listingName));
                    DEBUG_PRINT("Added new channel entry: " << name << std::endl);
                }
                else
                {
                    ERROR_PRINT("Found invalid listing type: " << listingTypeStr << " , skipping..." << std::endl);
                }
            }
            else
            {
                ERROR_PRINT("Found incomplete channel entry, skipping..." << std::endl);
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
        // Read file into stringstream
        std::stringstream buffer;
        buffer << ifs.rdbuf();

        // convert to c char and parse
        rapidjson::Document jsonDoc;
        jsonDoc.Parse(buffer.str().c_str());

        if (jsonDoc.IsObject() && jsonDoc.HasMember("ipdvrConfig"))
        {
            DEBUG_PRINT("Found a valid ipdvrConfig root tag in file: " << m_path << std::endl);
        }
        else
        {
            ERROR_PRINT("Error parsing config file: " << m_path << std::endl);
            return false;
        }

        const rapidjson::Value& rootNode = jsonDoc["ipdvrConfig"];
        if (rootNode.HasMember("channels"))
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
