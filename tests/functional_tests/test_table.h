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

    bool addUser(Json::JsonItem &data,
                 ErrorContainer &error);
    bool getUser(Json::JsonItem &resultItem,
                 const std::string &userID,
                 ErrorContainer &error,
                 const bool showHiddenValues = false);
    bool getAllUser(TableItem &resultItem,
                    ErrorContainer &error,
                    const bool showHiddenValues = false);
    bool deleteUser(const std::string &userID,
                    ErrorContainer &error);
};

}
}

#endif // TESTTABLE_H
