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

#include "listingupdater.h"

#include <ipdvr/debug.h>
#include <ipdvr/listingfetch_xmltvse.h>

namespace ipdvr {

ListingUpdater::ListingUpdater(std::shared_ptr<Config> spConfig, std::shared_ptr<ListingDb> spListingDb)
    : m_spConfig(spConfig),
      m_spListingDb(spListingDb)
{

}

void ListingUpdater::updateAll()
{
    const auto& channelDataList = m_spConfig->getChannelData();

    const ListingParse_Jsontv parseJsontv;

    for (const auto& channelData : channelDataList)
    {
        ListingFetch_XmltvSe fetcher(channelData, parseJsontv, m_spListingDb);
        INFO_PRINT("Updating programme listings for channel: " << channelData.name << std::endl);
        auto res = fetcher.fetch();

        for (const auto& programme : res.programmes)
        {
            m_spListingDb->insertProgramme(programme);
        }
        for (const auto& downloaded : res.downloadedFiles)
        {
            m_spListingDb->insertDownloaded(downloaded);
        }

        INFO_PRINT("Channel: " << channelData.name <<
                   " -- Downloaded " << res.downloadedFiles.size() << " files with "
                   << res.programmes.size() << " programmes" << std::endl);
    }

    m_spListingDb->deletePastProgrammes();
}

} // namespace ipdvr
