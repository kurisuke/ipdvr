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

#ifndef CHANNELDATA_H
#define CHANNELDATA_H

#include <list>
#include <string>

struct ChannelData
{
    enum class ListingType
    {
        Invalid = 0,
        JsonXmltvSe = 1
    };

    typedef std::list<ChannelData> StdList;

    std::string name;
    std::string streamUrl;
    ListingType listingType;
    std::string listingName;

    ChannelData(const std::string& c_name, const std::string& c_streamUrl,
                const ListingType& c_listingType, const std::string& c_listingName)
        : name(c_name),
          streamUrl(c_streamUrl),
          listingType(c_listingType),
          listingName(c_listingName) {}

    ChannelData()
        : name(""),
          streamUrl(""),
          listingType(ListingType::Invalid),
          listingName("") {}
};

#endif // CHANNELDATA_H
