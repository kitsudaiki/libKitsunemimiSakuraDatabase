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
const std::string
TestTable::addUser(const UserData &data,
                   ErrorContainer &error)
{
    const std::vector<std::string> values = { data.userName,
                                              data.pwHash,
                                              std::to_string(data.isAdmin)};
    return insertToDb(values, error);
}

/**
 * @brief UsersDatabase::getUser
 * @param userID
 * @param error
 * @return
 */
bool
TestTable::getUser(TableItem &resultItem,
                   const std::string &userID,
                   ErrorContainer &error)
{
    std::vector<RequestCondition> conditions;
    conditions.emplace_back("uuid", userID);
    return getFromDb(&resultItem, conditions, error);
}

/**
 * @brief UsersDatabase::getAllUser
 * @param error
 * @return
 */
bool
TestTable::getAllUser(TableItem &resultItem,
                      ErrorContainer &error)
{
    return getAllFromDb(&resultItem, error);
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
    conditions.emplace_back("uuid", userID);
    return deleteFromDb(conditions, error);
}

}
}
