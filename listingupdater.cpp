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

#include "debug.h"
#include "listingfetch_xmltvse.h"

ListingUpdater::ListingUpdater(std::shared_ptr<Config> spConfig)
    : m_spConfig(spConfig)
{

}

void ListingUpdater::updateAll()
{
    const auto& channelDataList = m_spConfig->getChannelData();

    for (const auto& channelData : channelDataList)
    {
        ListingFetch_XmltvSe fetcher(channelData);
        auto programmes = fetcher.fetch();
    }
}
