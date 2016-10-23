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

#ifndef IPDVR_LISTINGFETCH_XMLTVSE_H
#define IPDVR_LISTINGFETCH_XMLTVSE_H

#include <string>
#include <list>

#include <ipdvr/channeldata.h>
#include <ipdvr/ilistingfetch.h>
#include <ipdvr/listingdb.h>
#include <ipdvr/listingparse_jsontv.h>

namespace ipdvr {

class ListingFetch_XmltvSe : public IListingFetch
{
public:
    ListingFetch_XmltvSe(const ChannelData& channelData, const ListingParse_Jsontv& jsontvParser, std::shared_ptr<ListingDb> spListingDb);
    ~ListingFetch_XmltvSe();

    virtual ListingFetchResult fetch() const override;

private:
    std::list<std::string> generateUrls(unsigned int days) const;
    ListingFetchResult fetchUrl(const std::string& singleUrl) const;

    ChannelData m_channelData;
    std::shared_ptr<ListingDb> m_spListingDb;
    const ListingParse_Jsontv& m_jsontvParser;
};

} // namespace ipdvr

#endif // IPDVR_LISTINGFETCH_XMLTVSE_H
