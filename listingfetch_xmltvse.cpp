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

#include "listingfetch_xmltvse.h"

#include "debug.h"
#include "fetchjob_curl.h"

#include <chrono>
#include <curl/curl.h>
#include <ctime>

#include <sstream>

ListingFetch_XmltvSe::ListingFetch_XmltvSe(ChannelData channelData)
  : m_channelData(channelData)
{
    curl_global_init(CURL_GLOBAL_ALL);
}

ListingFetch_XmltvSe::~ListingFetch_XmltvSe()
{
    curl_global_cleanup();
}

void ListingFetch_XmltvSe::fetch()
{
    auto urls = generateUrls(7);

    for(auto& url : urls)
    {
        fetchUrl(url);
    }
}

std::shared_ptr<rapidjson::Document> ListingFetch_XmltvSe::fetchUrl(std::string singleUrl)
{
    std::unique_ptr<FetchJob_Curl> pFetcher(new FetchJob_Curl(singleUrl));

    bool res = pFetcher->run();

    if (res == false)
    {
        ERROR_PRINT("Fetch failed from URL: " << singleUrl << std::endl);
        return nullptr;
    }
    else
    {
        DEBUG_PRINT("Read " << pFetcher->getSize() << " bytes from URL: " << singleUrl << std::endl);
    }

    auto spJsonDoc = std::make_shared<rapidjson::Document>();
    spJsonDoc->Parse(pFetcher->getData());

    if (spJsonDoc->IsObject() && spJsonDoc->HasMember("jsontv"))
    {
        DEBUG_PRINT("Parsed a valid JSONTV document." << std::endl);
        return spJsonDoc;
    }
    else
    {
        ERROR_PRINT("JSON parse failed!" << std::endl);
        return nullptr;
    }
}

std::list<std::string> ListingFetch_XmltvSe::generateUrls(unsigned int days)
{
    auto now = std::chrono::system_clock::now();

    std::list<std::string> l;

    for(int i = 0; i <= days; i++)
    {
        time_t tt = std::chrono::system_clock::to_time_t(now + std::chrono::hours(24 * i));
        tm utc_tm = *gmtime(&tt);

        std::stringstream s;
        s << "http://json.xmltv.se/"
          << m_channelData.listingName << "_"
          << utc_tm.tm_year + 1900 << "-"
          << utc_tm.tm_mon + 1 << "-"
          << utc_tm.tm_mday
          << ".js.gz";

        l.push_back(s.str());
        DEBUG_PRINT(s.str() << std::endl);
    }

    return std::move(l);
}
