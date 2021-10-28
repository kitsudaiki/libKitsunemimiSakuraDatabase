/**
 * @file       sql_database.cpp
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

#include <libKitsunemimiSakuraDatabase/sql_database.h>

namespace Kitsunemimi
{
namespace Sakura
{

/**
 * @brief constructor
 */
SqlDatabase::SqlDatabase() {}

/**
 * @brief destructor
 */
SqlDatabase::~SqlDatabase() {}

/**
 * @brief create a sql-query to create a table
 *
 * @return created sql-query
 */
const std::string
SqlDatabase::createTableCreateQuery()
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
 * @param compare value to compare against the first comlumn
 *
 * @return created sql-query
 */
const std::string
SqlDatabase::createSelectQuery(const std::string &compare)
{
    const std::string command = "SELECT * from "
                                + m_tableName
                                + " WHERE "
                                + m_tableHeader.at(0).name
                                + "='"
                                + compare
                                + "';";
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
SqlDatabase::createInsertQuery(const std::vector<std::string> &values)
{
    std::string command  = "INSERT INTO ";
    command.append(m_tableName);
    command.append("(");

    // create fields
    for(uint32_t i = 0; i < m_tableHeader.size(); i++)
    {
        const DbHeaderEntry* entry = &m_tableHeader[i];
        if(i != 0) {
            command.append(" , ");
        }
        command.append(entry->name);
    }

    // create values
    command.append(") VALUES (");
    for(uint32_t i = 0; i < m_tableHeader.size(); i++)
    {
        if(i != 0) {
            command.append(" , ");
        }
        command.append("'");
        command.append(values.at(i));
        command.append("'");
    }

    command.append(" );");

    return command;
}

} // namespace Sakura
} // namespace Kitsunemimi
