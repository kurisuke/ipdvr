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

class ChannelData
{
public:
    enum class ListingType
    {
        Invalid = 0,
        JsonXmltvSe = 1
    };

    typedef std::list<ChannelData> StdList;

    ChannelData(const std::string& name, const std::string& streamUrl,
                const ListingType& listingType, const std::string& listingName);

    ChannelData();

    const std::string& getName();
    const std::string& getStreamUrl();
    const ListingType& getListingType();
    const std::string& getListingName();

    void setName(const std::string& name);
    void setStreamUrl(const std::string& streamUrl);
    void setListingType(const ListingType& listingType);
    void setListingName(const std::string& listingName);

private:
    std::string m_name;
    std::string m_streamUrl;
    ListingType m_listingType;
    std::string m_listingName;
};

#endif // CHANNELDATA_H
