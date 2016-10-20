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

#ifndef IPDVR_DEBUG_H
#define IPDVR_DEBUG_H

#include <iostream>
#include <cstring>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define ERROR_PRINT(x) do { std::cerr << "[" << __FILENAME__ << ":" << __LINE__ << "] " << x; } while (0)

#ifdef DEBUG
#   define DEBUG_PRINT(x) do { std::cout << "[" << __FILENAME__ << ":" << __LINE__ << "] " << x; } while (0)
#else
#   define DEBUG_PRINT(x)
#endif // DEBUG

#endif // IPDVR_DEBUG_H
