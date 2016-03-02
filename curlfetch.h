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

#ifndef CURLFETCH_H
#define CURLFETCH_H

#include <string>

#include <curl/curl.h>

class CurlFetch
{
public:
    CurlFetch(std::string & sUrl);
    ~CurlFetch();

    bool run();
    char* getData();
    size_t getSize();

private:
    static size_t cbWrite(void *src, size_t size, size_t nmemb, void *dest);

    struct FetchResult {
        char* pData;
        size_t size;
    };

    struct FetchResult m_fetchResult;

    static bool m_bCurlGlobalInit;
    CURL* m_hCurl;
};

#endif // CURLFETCH_H
