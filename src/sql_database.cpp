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
SqlDatabase::~SqlDatabase()
{
    closeDatabase();
}

/**
 * @brief initialize sqlite-database
 *
 * @param path file-path to sqlite-database
 * @param error reference for error-output
 *
 * @return true, if successfull, else false
 */
bool
SqlDatabase::initDatabase(const std::string &path,
                          Kitsunemimi::ErrorContainer &error)
{
    std::lock_guard<std::mutex> guard(m_lock);

    // check if database is already open
    if(m_isOpen)
    {
        LOG_DEBUG("Database already open");
        return true;
    }

    // init database
    if(m_db.initDB(path, error))
    {
        m_isOpen = true;
        m_path = path;

        return true;
    }

    return false;
}

/**
 * @brief close database-connectiono
 *
 * @return true, if successfull, else false
 */
bool
SqlDatabase::closeDatabase()
{
    std::lock_guard<std::mutex> guard(m_lock);

    // check if already closed
    if(m_isOpen == false) {
        return true;
    }

    // close
    if(m_db.closeDB())
    {
        m_isOpen = false;
        return true;
    }

    return false;
}

/**
 * @brief execute sql-query
 *
 * @param resultTable table-pointer for the result of the query
 * @param command queuy to execute
 * @param error reference for error-output
 *
 * @return true, if successfull, else false
 */
bool
SqlDatabase::execSqlCommand(TableItem* resultTable,
                            const std::string &command,
                            ErrorContainer &error)
{
    std::lock_guard<std::mutex> guard(m_lock);

    if(m_isOpen == false)
    {
        error.addMeesage("database not open");
        LOG_ERROR(error);
        return false;
    }

    LOG_DEBUG("run SQL-command: " + command);

    return m_db.execSqlCommand(resultTable, command, error);
}

} // namespace Sakura
} // namespace Kitsunemimi
