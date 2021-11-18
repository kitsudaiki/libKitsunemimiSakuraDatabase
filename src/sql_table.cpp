/**
 * @file       sql_table.cpp
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

#include <libKitsunemimiSakuraDatabase/sql_table.h>
#include <libKitsunemimiSakuraDatabase/sql_database.h>
#include <libKitsunemimiCommon/common_methods/string_methods.h>

#include <uuid/uuid.h>

namespace Kitsunemimi
{
namespace Sakura
{

/**
 * @brief constructor
 *
 * @param db pointer to database
 */
SqlTable::SqlTable(SqlDatabase* db)
{
    m_db = db;

    DbHeaderEntry userId;
    userId.name = "uuid";
    userId.maxLength = UUID_STR_LEN;
    userId.isPrimary = true;
    m_tableHeader.push_back(userId);
}

/**
 * @brief destructor
 */
SqlTable::~SqlTable() {}

/**
 * @brief initalize table
 *
 * @param error reference for error-output
 *
 * @return true, if successfuly or table already exist, else false
 */
bool
SqlTable::initTable(ErrorContainer &error)
{
    return m_db->execSqlCommand(nullptr, createTableCreateQuery(), error);
}

/**
 * @brief insert values into the table
 *
 * @param values string-list with values to insert
 * @param error reference for error-output
 *
 * @return uuid of the new entry, if successfull, else empty string
 */
const std::string
SqlTable::insertToDb(const std::vector<std::string> &values,
                     ErrorContainer &error)
{
    Kitsunemimi::TableItem resultItem;

    // create uuid
    char uuid[UUID_STR_LEN];
    uuid_t binaryUuid;
    uuid_generate_random(binaryUuid);
    uuid_unparse_lower(binaryUuid, uuid);

    // fill into string, but must be reduced by 1 to remove the escate-character
    std::string uuidString = std::string(uuid, UUID_STR_LEN - 1);
    Kitsunemimi::toLowerCase(uuidString);

    const bool ret = m_db->execSqlCommand(&resultItem,
                                          createInsertQuery(uuidString, values),
                                          error);
    if(ret == false) {
        return "";
    }

    return uuidString;
}

/**
 * @brief get all rows from table
 *
 * @param resultTable pointer to table for the resuld of the query
 * @param error reference for error-output
 *
 * @return true, if successfull, else false
 */
bool
SqlTable::getAllFromDb(TableItem *resultTable,
                       ErrorContainer &error)
{
    return m_db->execSqlCommand(resultTable, createSelectQuery("", ""), error);
}

/**
 * @brief get single row from table
 *
 * @param resultTable pointer to table for the resuld of the query
 * @param uuid uuid of the row to get
 * @param error reference for error-output
 *
 * @return true, if successfull, else false
 */
bool
SqlTable::getFromDb(TableItem *resultTable,
                    std::string uuid,
                    ErrorContainer &error)
{
    Kitsunemimi::toLowerCase(uuid);
    return getFromDb(resultTable, "uuid", uuid, error);
}

/**
 * @brief get one or more rows from table
 *
 * @param resultTable pointer to table for the resuld of the query
 * @param colName name of the column to compare
 * @param compare value for comparism
 * @param error reference for error-output
 *
 * @return true, if successfull, else false
 */
bool
SqlTable::getFromDb(TableItem* resultTable,
                    const std::string &colName,
                    const std::string &compare,
                    ErrorContainer &error)
{
    return m_db->execSqlCommand(resultTable, createSelectQuery(colName, compare), error);
}

/**
 * @brief delete row from database
 *
 * @param uuid uuid of the row to delete
 * @param error reference for error-output
 *
 * @return true, if successfull, else false
 */
bool
SqlTable::deleteFromDb(std::string uuid, ErrorContainer &error)
{
    Kitsunemimi::toLowerCase(uuid);
    return deleteFromDb("uuid", uuid, error);
}

/**
 * @brief delete one of more rows from database
 *
 * @param colName name of the column to compare
 * @param compare value for comparism
 * @param error reference for error-output
 *
 * @return true, if successfull, else false
 */
bool
SqlTable::deleteFromDb(const std::string &colName,
                       const std::string &compare,
                       ErrorContainer &error)
{
    Kitsunemimi::TableItem resultItem;
    return m_db->execSqlCommand(&resultItem, createDeleteQuery(colName, compare), error);
}

/**
 * @brief create a sql-query to create a table
 *
 * @return created sql-query
 */
const std::string
SqlTable::createTableCreateQuery()
{
    std::string command = "CREATE TABLE IF NOT EXISTS ";
    command.append(m_tableName);
    command.append(" (");

    // create all field of the table
    for(uint32_t i = 0; i < m_tableHeader.size(); i++)
    {
        const DbHeaderEntry* entry = &m_tableHeader[i];
        if(i != 0) {
            command.append(" , ");
        }
        command.append(entry->name + "  ");

        // set type of the value
        switch(entry->type)
        {
            case STRING_TYPE:
                if(entry->maxLength > 0)
                {
                    command.append("varchar(");
                    command.append(std::to_string(entry->maxLength));
                    command.append(") ");
                }
                else
                {
                   command.append("text ");
                }
                break;
            case INT_TYPE:
                command.append("int ");
                break;
            case BOOL_TYPE:
                command.append("bool ");
                break;
        }

        // set if key is primary key
        if(entry->isPrimary) {
            command.append("PRIMARY KEY ");
        }

        // set if value is not allowed to be null
        if(entry->allowNull == false) {
            command.append("NOT NULL ");
        }
    }

    command.append(");");

    return command;
}

/**
 * @brief create a sql-query to get a line from the table
 *
 * @param colName name of the column to compare
 * @param compare value to compare against the first comlumn
 *
 * @return created sql-query
 */
const std::string
SqlTable::createSelectQuery(const std::string &colName,
                            const std::string &compare)
{
    std::string command = "SELECT * from " + m_tableName;
    if(colName != "")
    {
        command.append(" WHERE ");
        command.append(colName);
        command.append("='");
        command.append(compare);
    }
    command.append("';");

    return command;
}

/**
 * @brief create a sql-query to insert values into the table
 *
 * @param values list of values to insert
 *
 * @return created sql-query
 */
const std::string
SqlTable::createInsertQuery(const std::string &uuid,
                            const std::vector<std::string> &values)
{
    std::string command  = "INSERT INTO ";
    command.append(m_tableName);
    command.append("(");
    command.append("uuid ");

    // create fields
    for(uint32_t i = 1; i < m_tableHeader.size(); i++)
    {
        const DbHeaderEntry* entry = &m_tableHeader[i];
        command.append(" , ");
        command.append(entry->name);
    }

    // create values
    command.append(") VALUES (");

    command.append("'");
    command.append(uuid);
    command.append("'");

    for(uint32_t i = 0; i < m_tableHeader.size() - 1; i++)
    {
        command.append(" , ");
        command.append("'");
        command.append(values.at(i));
        command.append("'");
    }

    command.append(" );");

    return command;
}

/**
 * @brief create query to delete rows from table
 *
 * @param colName name of the column to compare
 * @param compare value for comparism
 *
 * @return created sql-query
 */
const std::string
SqlTable::createDeleteQuery(const std::string &colName,
                            const std::string &compare)
{
    std::string command  = "DELETE FROM ";
    command.append(m_tableName);
    command.append(" WHERE ");
    command.append(colName);
    command.append("='");
    command.append(compare);
    command.append("';");

    return command;
}

} // namespace Sakura
} // namespace Kitsunemimi
