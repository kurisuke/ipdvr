/* ipdvr - DVR for IPTV streams
 *
 * Copyright (C) 2015  Peter Helbing
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

#include "fetchjob_curl.h"

#include "debug.h"

#include <iostream>

int main(void)
{
    std::string url = "http://json.xmltv.se/mdr.daserste.de_2016-03-01.js.gz";
    auto fetcher = FetchJob_Curl(url);

    bool res = fetcher.run();

    if (res == false)
    {
        DEBUG_PRINT("Fetch failed from URL: " << url << std::endl);
    }
    else
    {
        DEBUG_PRINT("Read " << fetcher.getSize() << " bytes from URL: " << url << std::endl);
    }

    return 0;
}
