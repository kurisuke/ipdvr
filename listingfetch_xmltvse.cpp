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

ListingFetch_XmltvSe::ListingFetch_XmltvSe(const ChannelData& channelData, const std::string &defaultLanguage)
  : m_channelData(channelData),
    m_defaultLanguage(defaultLanguage)
{
}

ListingFetch_XmltvSe::~ListingFetch_XmltvSe()
{
}

std::list<ProgrammeData> ListingFetch_XmltvSe::fetch()
{
    std::list<ProgrammeData> pd;

    auto urls = generateUrls(7);

    std::list<std::future<std::list<ProgrammeData>>> fList;

    for(auto& url : urls)
    {
        fList.push_back(std::async(std::launch::async,
                                   [url, this] { return parseListing(fetchUrl(url)); } ));
    }

    for(auto& fut : fList)
    {
        pd.splice(pd.end(), fut.get());
    }

    return pd;
}

std::shared_ptr<json> ListingFetch_XmltvSe::fetchUrl(const std::string& singleUrl)
{
    std::unique_ptr<FetchJob_Curl> pHeaderFetcher(new FetchJob_Curl(singleUrl));
    std::unique_ptr<FetchJob_Curl> pFetcher(new FetchJob_Curl(singleUrl));

    int filetime = pHeaderFetcher->getFiletime();

    bool res = pFetcher->download();

    if (res == false)
    {
        ERROR_PRINT("Fetch failed from URL: " << singleUrl << std::endl);
        return nullptr;
    }
    else
    {
        DEBUG_PRINT("Read " << pFetcher->getSize() << " bytes from URL: " << singleUrl << std::endl);
    }

    auto spJsonDoc = std::make_shared<json>(json::parse(pFetcher->getData()));

    if (spJsonDoc->is_object() && (spJsonDoc->find("jsontv") != spJsonDoc->end()))
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

std::list<ProgrammeData> ListingFetch_XmltvSe::parseListing(std::shared_ptr<json> spJsonDoc)
{
    if ((spJsonDoc != nullptr) &&
        (spJsonDoc->find("jsontv") != spJsonDoc->end()) &&
        ((*spJsonDoc)["jsontv"].find("programme") != (*spJsonDoc)["jsontv"].end()))
    {
        const auto& programmeRoot = (*spJsonDoc)["jsontv"]["programme"];

        std::list<ProgrammeData> programmeList;

        for (const auto& it : programmeRoot)
        {
            if (it.is_object())
            {
                if ((it.find("title") != it.end()) &&
                    (it.find("start") != it.end()) && (it["start"].is_string()) &&
                    (it.find("stop") != it.end()) && (it["stop"].is_string()))
                {
                    const std::string desc = (it.find("desc") != it.end()) ? getLocalizedString(it["desc"]) : "" ;

                    int iStart, iStop;
                    std::string sStart = it["start"];
                    std::string sStop = it["stop"];

                    try {
                      iStart = std::stoi(sStart);
                      iStop = std::stoi(sStop);
                    }
                    catch(std::invalid_argument& ){
                      ERROR_PRINT("Found incomplete programme entry, skipping..." << std::endl);
                      continue;
                    }
                    catch(std::out_of_range& ){
                      ERROR_PRINT("Found incomplete programme entry, skipping..." << std::endl);
                      continue;
                    }

                    programmeList.emplace_back(getLocalizedString(it["title"]),
                                               desc,
                                               iStart,
                                               iStop);

                    DEBUG_PRINT("Added entry:" << getLocalizedString(it["title"]) << std::endl);
                }
                else
                {
                    ERROR_PRINT("Found incomplete programme entry, skipping..." << std::endl);
                }
            }
        }

        return programmeList;
    }
    else
    {
        return std::list<ProgrammeData>();
    }
}

std::string ListingFetch_XmltvSe::getLocalizedString(const json &itemList, const std::string& language)
{
    if (!itemList.is_object())
    {
        ERROR_PRINT("Not an object (for language-specific strings)" << std::endl);
        return "";
    }

    if ((language != "") && (itemList.find(language) != itemList.end()) && itemList[language].is_string())
    {
        return itemList[language];
    }
    else if ((itemList.find(m_defaultLanguage) != itemList.end()) && itemList[m_defaultLanguage].is_string())
    {
        return itemList[m_defaultLanguage];
    }
    else
    {
        auto it = itemList.begin();
        if ((it != itemList.end()) && it->is_string())
        {
            return (*it);
        }
        else
        {
            return "";
        }
    }
}
