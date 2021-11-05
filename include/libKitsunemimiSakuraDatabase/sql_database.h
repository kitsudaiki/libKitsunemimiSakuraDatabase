/**
 * @file       sql_database.h
 *
 * @author     Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright  Apache License Version 2.0
 *
 *      Copyright 2021 Tobias Anker
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include <mutex>
#include <libKitsunemimiSqlite/sqlite.h>

namespace Kitsunemimi
{
namespace Sqlite {
class Sqlite;
}
namespace Sakura
{

class SqlDatabase
{
public:
    SqlDatabase();


    bool initDatabase(const std::string &path,
                      Kitsunemimi::ErrorContainer &error);
    bool closeDatabase();


    bool execSqlCommand(TableItem* resultTable,
                        const std::string &command,
                        ErrorContainer &error);

private:
    std::mutex m_lock;
    bool m_isOpen = false;
    std::string m_path = "";

    Kitsunemimi::Sqlite::Sqlite m_db;
};

}
}

#endif // SQLDATABASE_H
