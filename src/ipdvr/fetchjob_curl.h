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

#ifndef IPDVR_FETCHJOB_CURL_H
#define IPDVR_FETCHJOB_CURL_H

#include <string>

#include <curl/curl.h>

namespace ipdvr {

class FetchJob_Curl
{
public:
    FetchJob_Curl(const std::string & sUrl);
    ~FetchJob_Curl();

    bool download();
    int getFiletime();
    char* getData();
    size_t getSize();

private:
    static size_t cbWrite(void *src, size_t size, size_t nmemb, void *dest);
    static size_t cbDummy(void *src, size_t size, size_t nmemb, void *dest);

    struct FetchResult {
        char* pData;
        size_t size;
    };

    struct FetchResult m_fetchResult;

    CURL* m_hCurl;
};

} // namespace ipdvr

#endif // IPDVR_FETCHJOB_CURL_H
