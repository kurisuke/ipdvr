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
#include <memory>

ListingFetch::ListingFetch()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

ListingFetch::~ListingFetch()
{
    curl_global_cleanup();
}

bool ListingFetch::fetch(std::string singleUrl)
{
    std::unique_ptr<FetchJob_Curl> fetcher(new FetchJob_Curl(singleUrl));

    bool res = fetcher->run();

    if (res == false)
    {
        DEBUG_PRINT("Fetch failed from URL: " << singleUrl << std::endl);
        return false;
    }
    else
    {
        DEBUG_PRINT("Read " << fetcher->getSize() << " bytes from URL: " << singleUrl << std::endl);
        return true;
    }
}
