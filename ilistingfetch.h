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

#ifndef IPDVR_ILISTINGFETCH_H
#define IPDVR_ILISTINGFETCH_H

#include <list>

#include "programmedata.h"

struct DownloadedFile
{
    std::string name;
    int date;
};

struct ListingFetchResult
{
    std::list<ProgrammeData> programmes;
    std::list<DownloadedFile> downloadedFiles;
};

class IListingFetch
{
public:
    virtual ListingFetchResult fetch() const = 0;
};

#endif // IPDVR_ILISTINGFETCH_H
