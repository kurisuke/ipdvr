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
#include <ctime>
#include <future>
#include <sstream>

ListingFetch_XmltvSe::ListingFetch_XmltvSe(const ChannelData& channelData, const ListingParse_Jsontv& jsontvParser)
  : m_channelData(channelData),
    m_jsontvParser(jsontvParser)
{
}

ListingFetch_XmltvSe::~ListingFetch_XmltvSe()
{
}

ListingFetchResult ListingFetch_XmltvSe::fetch() const
{
    std::list<ProgrammeData> pd;
    std::list<DownloadedFile> df;
    std::list<std::future<ListingFetchResult>> fList;

    const auto urls = generateUrls(7);

    for(auto& url : urls)
    {
        fList.push_back(std::async(std::launch::async,
                                   [url, this] { return fetchUrl(url); } ));
    }

    for(auto& fut : fList)
    {
        auto fetchRes = fut.get();

        pd.splice(pd.end(), fetchRes.programmes);
        df.splice(df.end(), fetchRes.downloadedFiles);
    }

    return {pd, df};
}

ListingFetchResult ListingFetch_XmltvSe::fetchUrl(const std::string& singleUrl) const
{
    FetchJob_Curl headerFetcher(singleUrl);
    FetchJob_Curl fetcher(singleUrl);

    int filetime = headerFetcher.getFiletime();

    bool res = fetcher.download();

    if (res == false)
    {
        ERROR_PRINT("Fetch failed from URL: " << singleUrl << std::endl);
        return {std::list<ProgrammeData>(), std::list<DownloadedFile>()};
    }
    else
    {
        DEBUG_PRINT("Read " << fetcher.getSize() << " bytes from URL: " << singleUrl << std::endl);
    }

    std::list<DownloadedFile> l { {singleUrl, filetime} };

    return { m_jsontvParser.parseListing(fetcher.getData()), l};
}

std::list<std::string> ListingFetch_XmltvSe::generateUrls(unsigned int days) const
{
    const auto now = std::chrono::system_clock::now();

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
