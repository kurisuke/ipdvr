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

#ifndef IPDVR_LISTINGUPDATER_H
#define IPDVR_LISTINGUPDATER_H

#include <memory>

#include "config.h"
#include "listingdb.h"

namespace ipdvr {

class ListingUpdater
{
public:
    ListingUpdater(std::shared_ptr<Config> spConfig, std::shared_ptr<ListingDb> spListingDb);

    void updateAll();

private:
    std::shared_ptr<Config> m_spConfig;
    std::shared_ptr<ListingDb> m_spListingDb;
};

} // namespace ipdvr

#endif // IPDVR_LISTINGUPDATER_H
