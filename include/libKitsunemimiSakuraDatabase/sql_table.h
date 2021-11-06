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

#ifndef KITSUNEMIMI_SAKURA_DATABASE_SQL_DATABASE_H
#define KITSUNEMIMI_SAKURA_DATABASE_SQL_DATABASE_H

#include <vector>
#include <string>
#include <uuid/uuid.h>

#include <libKitsunemimiCommon/common_items/data_items.h>
#include <libKitsunemimiCommon/logger.h>

namespace Kitsunemimi
{
namespace Sakura
{
class SqlDatabase;

class SqlTable
{
public:
    SqlTable(SqlDatabase* db);
    virtual ~SqlTable();

protected:
    enum DbVataValueTypes
    {
        STRING_TYPE = 0,
        INT_TYPE = 1,
        BOOL_TYPE = 2,
    };

    struct DbHeaderEntry
    {
        std::string name = "";
        int maxLength = -1;
        DbVataValueTypes type = STRING_TYPE;
        bool isPrimary = false;
        bool allowNull = false;
    };

    std::vector<DbHeaderEntry> m_tableHeader;
    std::string m_tableName = "";


    bool initTable(Kitsunemimi::ErrorContainer &error);

    const std::string insertToDb(const std::vector<std::string> &values,
                                 ErrorContainer &error);
    bool getAllFromDb(TableItem* resultTable,
                      ErrorContainer &error);
    bool getFromDb(TableItem* resultTable,
                   std::string uuid,
                   ErrorContainer &error);
    bool getFromDb(TableItem* resultTable,
                   const std::string &colName,
                   const std::string &compare,
                   ErrorContainer &error);
    bool deleteFromDb(std::string uuid,
                      ErrorContainer &error);
    bool deleteFromDb(const std::string &colName,
                      const std::string &compare,
                      ErrorContainer &error);
private:
    SqlDatabase* m_db = nullptr;

    const std::string createTableCreateQuery();
    const std::string createSelectQuery(const std::string &colName,
                                        const std::string &compare);
    const std::string createInsertQuery(const std::string &uuid,
                                        const std::vector<std::string> &values);
    const std::string createDeleteQuery(const std::string &colName,
                                        const std::string &compare);
};

} // namespace Sakura
} // namespace Kitsunemimi

#endif // KITSUNEMIMI_SAKURA_DATABASE_SQL_DATABASE_H
