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

    for(auto& url : urls)
    {
        if(pd.empty())
        {
            pd = parseListing(fetchUrl(url));
        }
        else
        {
            pd.splice(pd.end(), parseListing(fetchUrl(url)));
        }
    }

    return pd;
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

std::list<ProgrammeData> ListingFetch_XmltvSe::parseListing(std::shared_ptr<rapidjson::Document> spJsonDoc)
{
    if ((spJsonDoc != nullptr) && (spJsonDoc->HasMember("jsontv")) && ((*spJsonDoc)["jsontv"].HasMember("programme")))
    {
        const auto& programmeRoot = (*spJsonDoc)["jsontv"]["programme"];

        std::list<ProgrammeData> programmeList;

        for (auto it = programmeRoot.Begin(); it != programmeRoot.End(); ++it)
        {
            if (it->IsObject())
            {
                if ((it->HasMember("title")) &&
                    (it->HasMember("start")) && (*it)["start"].IsString() &&
                    (it->HasMember("stop")) && (*it)["stop"].IsString())
                {
                    const std::string desc = it->HasMember("desc") ? getLocalizedString((*it)["desc"]) : "" ;

                    int iStart, iStop;

                    try {
                      iStart = std::stoi((*it)["start"].GetString());
                      iStop = std::stoi((*it)["stop"].GetString());
                    }
                    catch(std::invalid_argument& ){
                      ERROR_PRINT("Found incomplete programme entry, skipping..." << std::endl);
                      continue;
                    }
                    catch(std::out_of_range& ){
                      ERROR_PRINT("Found incomplete programme entry, skipping..." << std::endl);
                      continue;
                    }

                    programmeList.emplace_back(getLocalizedString((*it)["title"]),
                                               desc,
                                               iStart,
                                               iStop);

                    DEBUG_PRINT("Added entry:" << getLocalizedString((*it)["title"]) << std::endl);
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

std::string ListingFetch_XmltvSe::getLocalizedString(const rapidjson::Value& itemList, const std::string& language)
{
    if (!itemList.IsObject())
    {
        ERROR_PRINT("Not an object (for language-specific strings)" << std::endl);
        return "";
    }

    if ((language != "") && (itemList.HasMember(language.c_str())) && itemList[language.c_str()].IsString())
    {
        return itemList[language.c_str()].GetString();
    }
    else if ((itemList.HasMember(m_defaultLanguage.c_str())) && itemList[m_defaultLanguage.c_str()].IsString())
    {
        return itemList[m_defaultLanguage.c_str()].GetString();
    }
    else
    {
        auto it = itemList.Begin();
        if ((it != itemList.End()) && it->IsString())
        {
            return it->GetString();
        }
        else
        {
            return "";
        }
    }
}
