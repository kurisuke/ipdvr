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

#include "fetchjob_curl.h"

#include <cstdlib>
#include <cstring>

FetchJob_Curl::FetchJob_Curl(const std::string & sUrl)
    : m_fetchResult(),
      m_hCurl(0)
{
    m_hCurl = curl_easy_init();

    curl_easy_setopt(m_hCurl, CURLOPT_URL, sUrl.c_str());
    curl_easy_setopt(m_hCurl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(m_hCurl, CURLOPT_WRITEFUNCTION, cbWrite);

    m_fetchResult.pData = (char*) malloc(1);
    m_fetchResult.size = 0;
    curl_easy_setopt(m_hCurl, CURLOPT_WRITEDATA, (void*) &m_fetchResult);
}

FetchJob_Curl::~FetchJob_Curl()
{
    curl_easy_cleanup(m_hCurl);

    free(m_fetchResult.pData);
}

bool FetchJob_Curl::download()
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

int FetchJob_Curl::getFiletime()
{
    curl_easy_setopt(m_hCurl, CURLOPT_NOBODY, 1L);
    /* Ask for filetime */
    curl_easy_setopt(m_hCurl, CURLOPT_FILETIME, 1L);
    /* No header output: TODO 14.1 http-style HEAD output for ftp */
    curl_easy_setopt(m_hCurl, CURLOPT_HEADERFUNCTION, cbDummy);
    curl_easy_setopt(m_hCurl, CURLOPT_HEADER, 0L);


    CURLcode res = curl_easy_perform(m_hCurl);

    if (res == CURLE_OK)
    {
        int filetime;
        res = curl_easy_getinfo(m_hCurl, CURLINFO_FILETIME, &filetime);
        if ((res == CURLE_OK) && (filetime >= 0))
        {
            return filetime;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

size_t FetchJob_Curl::getSize()
{
    return m_fetchResult.size;
}

char* FetchJob_Curl::getData()
{
    return m_fetchResult.pData;
}

size_t FetchJob_Curl::cbWrite(void *src, size_t size, size_t nmemb, void *dest)
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

size_t FetchJob_Curl::cbDummy(void *ptr, size_t size, size_t nmemb, void *data)
{
  (void)ptr;
  (void)data;
  return (size_t)(size * nmemb);
}
