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

#ifndef IPDVR_CONFIG_H
#define IPDVR_CONFIG_H

#include "channeldata.h"

#include <string>

class Config
{
public:
    Config(std::string path);
    ~Config();

    bool parse();

    ChannelData getChannelData(const std::string &name);

private:

    std::string m_path;

    ChannelData::StdList m_channelDataList;
};

#endif // IPDVR_CONFIG_H
