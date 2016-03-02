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

#include "curlfetch.h"

#include <cstdlib>
#include <cstring>

bool CurlFetch::m_bCurlGlobalInit = false;

CurlFetch::CurlFetch(std::string & sUrl)
    : m_fetchResult(),
      m_hCurl(0)
{
    if (m_bCurlGlobalInit == false)
    {
         curl_global_init(CURL_GLOBAL_ALL);
         m_bCurlGlobalInit = true;
    }

    m_hCurl = curl_easy_init();

    curl_easy_setopt(m_hCurl, CURLOPT_URL, sUrl.c_str());
    curl_easy_setopt(m_hCurl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(m_hCurl, CURLOPT_WRITEFUNCTION, cbWrite);

    m_fetchResult.pData = (char*) malloc(1);
    m_fetchResult.size = 0;
    curl_easy_setopt(m_hCurl, CURLOPT_WRITEDATA, (void*) &m_fetchResult);
}

CurlFetch::~CurlFetch()
{
    curl_easy_cleanup(m_hCurl);

    free(m_fetchResult.pData);
}

bool CurlFetch::run()
{
    CURLcode res = curl_easy_perform(m_hCurl);

    if (res != CURLE_OK)
    {
        return false;
    }
    else
    {
        return true;
    }
}

size_t CurlFetch::getSize()
{
    return m_fetchResult.size;
}

char* CurlFetch::getData()
{
    return m_fetchResult.pData;
}

size_t CurlFetch::cbWrite(void *src, size_t size, size_t nmemb, void *dest)
{
    size_t realsize = size * nmemb;
    struct FetchResult* fetchResult = (struct FetchResult*) dest;

    fetchResult->pData = (char*) realloc(fetchResult->pData, fetchResult->size + realsize + 1);
    if (fetchResult->pData == NULL)
    {
        // out of memory, do error handling...
        return 0;
    }

    memcpy(&(fetchResult->pData[fetchResult->size]), src, realsize);
    fetchResult->size += realsize;
    fetchResult->pData[fetchResult->size] = 0;

    return realsize;
}
