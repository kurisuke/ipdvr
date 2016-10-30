#include <gtest/gtest.h>

#include <ipdvr/listingdb.h>

#include <cstdio>
#include <string>

namespace ipdvr {

class ListingDb_Test : public ::testing::Test
{
protected:

    virtual void SetUp() {
        const std::string dbFile = "/tmp/ipdvr_listingdb_test.db";
        remove(dbFile.c_str());
        db = std::unique_ptr<ListingDb>(new ListingDb(dbFile));
    }

    std::unique_ptr<ListingDb> db;
};

} // namespace ipdvr
