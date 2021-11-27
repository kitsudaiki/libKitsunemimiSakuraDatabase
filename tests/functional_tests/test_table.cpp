#include "test_table.h"

#include <libKitsunemimiSakuraDatabase/sql_table.h>
#include <libKitsunemimiSakuraDatabase/sql_database.h>

namespace Kitsunemimi
{
namespace Sakura
{

TestTable::TestTable(Kitsunemimi::Sakura::SqlDatabase* db)
    : SqlTable(db)
{
    m_tableName = "users";

    DbHeaderEntry userName;
    userName.name = "user_name";
    userName.maxLength = 256;
    m_tableHeader.push_back(userName);

    DbHeaderEntry pwHash;
    pwHash.name = "pw_hash";
    pwHash.maxLength = 64;
    pwHash.hide = true;
    m_tableHeader.push_back(pwHash);

    DbHeaderEntry isAdmin;
    isAdmin.name = "is_admin";
    isAdmin.type = BOOL_TYPE;
    m_tableHeader.push_back(isAdmin);
}

TestTable::~TestTable()
{

}

/**
 * @brief Users::addUser
 * @param data
 * @param errorMessage
 * @return
 */
bool
TestTable::addUser(Kitsunemimi::Json::JsonItem &data,
                   ErrorContainer &error)
{
    return insertToDb(data, error);
}

/**
 * @brief UsersDatabase::getUser
 * @param userID
 * @param error
 * @return
 */
bool
TestTable::getUser(Kitsunemimi::Json::JsonItem &resultItem,
                   const std::string &userID,
                   ErrorContainer &error,
                   const bool withHideValues)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("user_name", userID);
    return getFromDb(resultItem, conditions, error, withHideValues);
}

/**
 * @brief UsersDatabase::getAllUser
 * @param error
 * @return
 */
bool
TestTable::getAllUser(TableItem &resultItem,
                      ErrorContainer &error,
                      const bool showHiddenValues)
{
    return getAllFromDb(resultItem, error, showHiddenValues);
}

/**
 * @brief UsersDatabase::deleteUser
 * @param userID
 * @param error
 * @return
 */
bool
TestTable::deleteUser(const std::string &userID,
                      ErrorContainer &error)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("user_name", userID);
    return deleteFromDb(conditions, error);
}

}
}
