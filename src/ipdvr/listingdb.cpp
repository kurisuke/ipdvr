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

#include "listingdb.h"

#include "debug.h"

#include <algorithm>
#include "contrib/sqlite3pp/sqlite3pp.h"
#include <cstdlib>
#include <mutex>
#include <sys/stat.h>
#include <sys/types.h>

namespace ipdvr {

class ListingDb::Impl
{
    public:
        Impl();
        ~Impl();
        Impl(const Impl&) = delete;
        Impl& operator=(const Impl&) = delete;

        bool insertProgramme(const ProgrammeData& programme);
        bool insertDownloaded(const DownloadedFile& downloaded);

        bool deletePastProgrammes();

        int getDownloadedTime(const std::string& url);

    private:
        std::unique_ptr<sqlite3pp::database> m_upDb;
        bool bDbOpen = false;
        std::mutex m_mtxDb;

        void createProgrammesTable();
        void createDownloadedTable();
        bool createPath( mode_t mode, const std::string rootPath, const std::string path );

        size_t hashProgramme(const ProgrammeData& programme) const;
};

ListingDb::ListingDb()
  : m_upImpl(new ListingDb::Impl())
{
}

ListingDb::~ListingDb()
{
}

bool ListingDb::insertProgramme(const ProgrammeData &programme)
{
    return m_upImpl->insertProgramme(programme);
}

bool ListingDb::insertDownloaded(const DownloadedFile &downloaded)
{
    return m_upImpl->insertDownloaded(downloaded);
}

bool ListingDb::deletePastProgrammes()
{
    return m_upImpl->deletePastProgrammes();
}

int ListingDb::getDownloadedTime(const std::string& url)
{
    return m_upImpl->getDownloadedTime(url);
}

ListingDb::Impl::Impl()
{
    const std::string baseDir = getenv("HOME");
    const std::string subDir = ".local/share/ipdvr";
    const std::string dbFile = baseDir + "/" + subDir + "/listing.db";

    createPath(S_IRWXU, baseDir, subDir);
    m_upDb = std::unique_ptr<sqlite3pp::database>(new sqlite3pp::database(dbFile.c_str()));

    // check for table programmes and recreate if necessary
    try
    {
        sqlite3pp::query qry(*m_upDb, "SELECT * FROM programmes");
    }
    catch (sqlite3pp::database_error &)
    {
        INFO_PRINT("Creating table: programmes" << std::endl);
        createProgrammesTable();
    }

    try
    {
        sqlite3pp::query qry(*m_upDb, "SELECT id, channel, title, desc, start, stop FROM programmes");
    }
    catch (sqlite3pp::database_error &)
    {
        INFO_PRINT("Recreated outdated table: programmes" << std::endl);
        m_upDb->execute("DROP TABLE programmes");
        createProgrammesTable();
    }

    // check for table programmes and recreate if necessary
    try
    {
        sqlite3pp::query qry(*m_upDb, "SELECT * FROM downloaded");
    }
    catch (sqlite3pp::database_error &)
    {
        INFO_PRINT("Creating table: downloaded" << std::endl);
        createDownloadedTable();
    }

    try
    {
        sqlite3pp::query qry(*m_upDb, "SELECT url, time FROM downloaded");
    }
    catch (sqlite3pp::database_error &)
    {
        INFO_PRINT("Recreated outdated table: downloaded" << std::endl);
        m_upDb->execute("DROP TABLE downloaded");
        createDownloadedTable();
    }
}

ListingDb::Impl::~Impl()
{
}

void ListingDb::Impl::createProgrammesTable()
{
    m_upDb->execute("CREATE TABLE programmes (id INTEGER PRIMARY KEY NOT NULL,"
                    "channel VARCHAR NOT NULL, title VARCHAR NOT NULL, desc VARCHAR,"
                    "start INTEGER NOT NULL, stop INTEGER NOT NULL)");
}

void ListingDb::Impl::createDownloadedTable()
{
    m_upDb->execute("CREATE TABLE downloaded (url VARCHAR PRIMARY KEY NOT NULL, time INTEGER NOT NULL)");
}

bool ListingDb::Impl::createPath( mode_t mode, const std::string rootPath, const std::string path )
{
    struct stat st;

    for( std::string::const_iterator iter = path.begin() ; iter != path.end(); )
    {
         std::string::const_iterator newIter = std::find(iter, path.end(), '/');
         std::string newPath = rootPath + "/" + std::string(path.begin(), newIter);

         if( stat( newPath.c_str(), &st) != 0)
         {
             if( mkdir( newPath.c_str(), mode) != 0 && errno != EEXIST )
             {
                std::cout << "cannot create folder [" << newPath << "] : " << strerror(errno) << std::endl;
                return false;
             }
         }
         else if( !S_ISDIR(st.st_mode) )
         {
             errno = ENOTDIR;
             std:: cout << "path [" << newPath << "] not a dir " << std::endl;
             return false;
         }

         iter = newIter;
         if( newIter != path.end() )
             ++iter;
    }
    return true;
}

bool ListingDb::Impl::insertProgramme(const ProgrammeData &programme)
{
    std::lock_guard<std::mutex> lock(m_mtxDb);

    sqlite3pp::command cmd(*m_upDb, "REPLACE INTO programmes (id, channel, title, desc, start, stop) VALUES "
                                  "(?, ?, ?, ?, ?, ?)");
    cmd.bind(1, (long long int) hashProgramme(programme));
    cmd.bind(2, programme.channel, sqlite3pp::nocopy);
    cmd.bind(3, programme.title, sqlite3pp::nocopy);
    cmd.bind(4, programme.desc, sqlite3pp::nocopy);
    cmd.bind(5, programme.startSeconds());
    cmd.bind(6, programme.stopSeconds());

    return cmd.execute();
}

bool ListingDb::Impl::insertDownloaded(const DownloadedFile& downloaded)
{
    std::lock_guard<std::mutex> lock(m_mtxDb);

    sqlite3pp::command cmd(*m_upDb, "REPLACE INTO downloaded (url, time) VALUES (?, ?)");
    cmd.bind(1, downloaded.name, sqlite3pp::nocopy);
    cmd.bind(2, downloaded.time);

    return cmd.execute();
}

bool ListingDb::Impl::deletePastProgrammes()
{
    std::lock_guard<std::mutex> lock(m_mtxDb);

    int now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    sqlite3pp::command cmd(*m_upDb, "DELETE FROM programmes WHERE stop < ?");
    cmd.bind(1, now);

    return cmd.execute();
}

size_t ListingDb::Impl::hashProgramme(const ProgrammeData& programme) const
{
    std::hash<std::string> hash_fn;

    return hash_fn(programme.channel + programme.title +
                   std::to_string(programme.startSeconds()) + std::to_string(programme.stopSeconds()));
}

int ListingDb::Impl::getDownloadedTime(const std::string& url)
{
    std::lock_guard<std::mutex> lock(m_mtxDb);

    int dlTime = -1;
    std::string sQry = "SELECT time FROM downloaded WHERE url = '" + url + "'";
    sqlite3pp::query qry(*m_upDb, sQry.c_str());

    for (auto row : qry)
    {
        dlTime = row.get<int>(0);
    }

    return dlTime;
}

} // namespace ipdvr
