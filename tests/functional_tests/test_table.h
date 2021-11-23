#ifndef TESTTABLE_H
#define TESTTABLE_H

#include <libKitsunemimiSakuraDatabase/sql_table.h>

namespace Kitsunemimi
{
namespace Sakura
{
class SqlDatabase;

class TestTable :
        public Kitsunemimi::Sakura::SqlTable
{
public:
    TestTable(Kitsunemimi::Sakura::SqlDatabase* db);
    ~TestTable();

    struct UserData
    {
        std::string userId = "";
        std::string userName = "";
        std::string pwHash = "";
        bool isAdmin = false;
    };

    const std::string addUser(const UserData &data,
                              ErrorContainer &error);
    bool getUser(TableItem &resultItem,
                 const std::string &userID,
                 ErrorContainer &error);
    bool getAllUser(TableItem &resultItem,
                    ErrorContainer &error);
    bool deleteUser(const std::string &userID,
                    ErrorContainer &error);
};

}
}

#endif // TESTTABLE_H
