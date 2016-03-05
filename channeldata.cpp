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

#include "channeldata.h"

ChannelData::ChannelData(const std::__cxx11::string &name,
                         const std::__cxx11::string &streamUrl,
                         const ListingType &listingType,
                         const std::__cxx11::string &listingName)
    : m_name(name),
      m_streamUrl(streamUrl),
      m_listingType(listingType),
      m_listingName(listingName)
{
}

const std::string& ChannelData::getName()
{
    return m_name;
}

const std::string& ChannelData::getStreamUrl()
{
    return m_streamUrl;
}

const ChannelData::ListingType& ChannelData::getListingType()
{
    return m_listingType;
}

const std::string& ChannelData::getListingName()
{
    return m_listingName;
}

void ChannelData::setName(const std::string &name)
{
    m_name = name;
}

void ChannelData::setStreamUrl(const std::string &streamUrl)
{
    m_streamUrl = streamUrl;
}

void ChannelData::setListingType(const ChannelData::ListingType &listingType)
{
    m_listingType = listingType;
}

void ChannelData::setListingName(const std::string &listingName)
{
    m_listingName = listingName;
}
