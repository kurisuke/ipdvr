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

#include "listingfetch.h"

#include "debug.h"
#include "fetchjob_curl.h"

#include <curl/curl.h>

ListingFetch::ListingFetch()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

ListingFetch::~ListingFetch()
{
    curl_global_cleanup();
}

std::shared_ptr<rapidjson::Document> ListingFetch::fetch(std::string singleUrl)
{
    std::unique_ptr<FetchJob_Curl> pFetcher(new FetchJob_Curl(singleUrl));

    bool res = pFetcher->run();

    if (res == false)
    {
        DEBUG_PRINT("Fetch failed from URL: " << singleUrl << std::endl);
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
        DEBUG_PRINT("JSON parse failed!" << std::endl);
        return nullptr;
    }
}
