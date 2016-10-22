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

std::list<ProgrammeData> ListingFetch_XmltvSe::fetch() const
{
    std::list<ProgrammeData> pd;
    std::list<std::future<std::list<ProgrammeData>>> fList;

    const auto urls = generateUrls(7);

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

json ListingFetch_XmltvSe::fetchUrl(const std::string& singleUrl) const
{
    FetchJob_Curl headerFetcher(singleUrl);
    FetchJob_Curl fetcher(singleUrl);

    int filetime = headerFetcher.getFiletime();

    bool res = fetcher.download();

    if (res == false)
    {
        ERROR_PRINT("Fetch failed from URL: " << singleUrl << std::endl);
        return nullptr;
    }
    else
    {
        DEBUG_PRINT("Read " << fetcher.getSize() << " bytes from URL: " << singleUrl << std::endl);
    }

    const auto jsonDoc = json::parse(fetcher.getData());

    if (jsonDoc.is_object() && (jsonDoc.find("jsontv") != jsonDoc.end()))
    {
        DEBUG_PRINT("Parsed a valid JSONTV document." << std::endl);
        return jsonDoc;
    }
    else
    {
        ERROR_PRINT("JSON parse failed!" << std::endl);
        return nullptr;
    }
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

std::list<ProgrammeData> ListingFetch_XmltvSe::parseListing(const json& jsonDoc) const
{
    if ((jsonDoc == nullptr))
    {
        ERROR_PRINT("Invalid programme listing!" << std::endl);
        return std::list<ProgrammeData>();
    }

    try
    {
        jsonDoc.at("jsontv").at("programme");
    }
    catch (std::out_of_range&)
    {
        ERROR_PRINT("Invalid programme listing!" << std::endl);
        return std::list<ProgrammeData>();
    }

    const auto& programmeRoot = jsonDoc.at("jsontv").at("programme");
    std::list<ProgrammeData> programmeList;

    for (const auto& p : programmeRoot)
    {
        try
        {
            const auto title = getLocalizedString(p.at("title"));
            const auto desc = (p.find("desc") != p.end()) ? getLocalizedString(p["desc"]) : "";
            const auto start = std::stoi(p.at("start").get<std::string>());
            const auto stop = std::stoi(p.at("stop").get<std::string>());

            programmeList.emplace_back(title, desc, start, stop);
            DEBUG_PRINT("Added entry:" << title << std::endl);
        }
        catch (std::invalid_argument& )
        {
          ERROR_PRINT("Found incomplete programme entry, skipping..." << std::endl);
        }
        catch (std::out_of_range& )
        {
          ERROR_PRINT("Found incomplete programme entry, skipping..." << std::endl);
        }
        catch (std::domain_error& )
        {
          ERROR_PRINT("Found incomplete programme entry, skipping..." << std::endl);
        }
    }

    return programmeList;
}

std::string ListingFetch_XmltvSe::getLocalizedString(const json &itemList, const std::string& language) const
{
    const std::string ret = "";

    if (!itemList.is_object())
    {
        ERROR_PRINT("Not an object (for language-specific strings)" << std::endl);
        return "";
    }

    if ((language != "") && (itemList.find(language) != itemList.end()))
    {
        return itemList.at(language).get<std::string>();
    }
    else if ((m_defaultLanguage != "") && (itemList.find(m_defaultLanguage) != itemList.end()))
    {
        return itemList.at(m_defaultLanguage).get<std::string>();
    }
    else if(ret.size() > 0)
    {
        return itemList.begin()->get<std::string>();
    }
    else
    {
        return "";
    }
}
