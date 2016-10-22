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

#include "listingparse_jsontv.h"

#include "debug.h"

namespace ipdvr {

ListingParse_Jsontv::ListingParse_Jsontv(const std::string &defaultLanguage)
 : m_defaultLanguage(defaultLanguage)
{
}

ListingParse_Jsontv::~ListingParse_Jsontv()
{
}

std::list<ProgrammeData> ListingParse_Jsontv::parseListing(const std::string& data) const
{
    const auto jsonDoc = json::parse(data);

    try
    {
        jsonDoc.at("jsontv").at("programme");
    }
    catch (std::logic_error&)
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

std::string ListingParse_Jsontv::getLocalizedString(const json &itemList, const std::string &language) const
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

} // namespace ipdvr
