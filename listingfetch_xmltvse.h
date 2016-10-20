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

#ifndef IPDVR_LISTINGFETCH_XMLTVSE_H
#define IPDVR_LISTINGFETCH_XMLTVSE_H

#include <memory>
#include "rapidjson/document.h"
#include <string>
#include <list>

#include "channeldata.h"
#include "programmedata.h"

class ListingFetch_XmltvSe
{
public:
    ListingFetch_XmltvSe(const ChannelData& channelData, const std::string& defaultLanguage="de");
    ~ListingFetch_XmltvSe();

    std::list<ProgrammeData> fetch();

private:
    std::list<std::string> generateUrls(unsigned int days);
    std::shared_ptr<rapidjson::Document> fetchUrl(std::string singleUrl);
    std::list<ProgrammeData> parseListing(std::shared_ptr<rapidjson::Document> spJsonDoc);

    std::string getLocalizedString(const rapidjson::Value& itemList, const std::string& language="");

    ChannelData m_channelData;
    std::string m_defaultLanguage;
};

#endif // IPDVR_LISTINGFETCH_XMLTVSE_H
