#include "sql_table_test.h"

#include <libKitsunemimiSakuraDatabase/sql_database.h>
#include <libKitsunemimiSakuraDatabase/sql_table.h>

#include <test_table.h>

namespace Kitsunemimi
{
namespace Sakura
{

SqlTable_Test::SqlTable_Test()
    : Kitsunemimi::CompareTestHelper("SqlTable_Test")
{
    initTest();

    initDatabase_test();
    initTable_test();

    create_test();
    get_test();
    getAll_test();
    delete_test();
}

/**
 * @brief initTest
 */
void
SqlTable_Test::initTest()
{
    m_filePath = "/tmp/testdb.db";
    deleteFile();
}

/**
 * @brief initDB_test
 */
void
SqlTable_Test::initDatabase_test()
{
    Kitsunemimi::ErrorContainer error;
    m_db = new SqlDatabase();
    TEST_EQUAL(m_db->initDatabase(m_filePath, error), true);
}

/**
 * @brief initTable_test
 */
void
SqlTable_Test::initTable_test()
{
    m_table = new TestTable(m_db);
    ErrorContainer error;
    m_table->initTable(error);
}

/**
 * @brief create_test
 */
void
SqlTable_Test::create_test()
{
    TestTable::UserData testData;
    ErrorContainer error;

    testData.userName = "user0815";
    testData.pwHash = "secret";
    testData.isAdmin = true;

    m_uuid = m_table->addUser(testData, error);
    TEST_NOT_EQUAL(m_uuid.size(), 0);

    testData.userName = "another user";
    testData.pwHash = "secret2";
    testData.isAdmin = false;

    m_table->addUser(testData, error);
}

/**
 * @brief get_test
 */
void
SqlTable_Test::get_test()
{
    TableItem result;
    ErrorContainer error;

    TEST_EQUAL(m_table->getUser(result, m_uuid, error), true);
    TEST_EQUAL(result.getNumberOfRows(), 1);
    TEST_EQUAL(result.getNumberOfColums(), 4);
}

/**
 * @brief getAll_test
 */
void
SqlTable_Test::getAll_test()
{
    TableItem result;
    ErrorContainer error;

    TEST_EQUAL(m_table->getAllUser(result, error), true);
    TEST_EQUAL(result.getNumberOfRows(), 2);
    TEST_EQUAL(result.getNumberOfColums(), 4);
}

/**
 * @brief delete_test
 */
void
SqlTable_Test::delete_test()
{
    TableItem result;
    ErrorContainer error;

    TEST_EQUAL(m_table->deleteUser(m_uuid, error), true);
    m_table->getAllUser(result, error);
    TEST_EQUAL(result.getNumberOfRows(), 1);
}

/**
 * common usage to delete test-file
 */
void
SqlTable_Test::deleteFile()
{
    std::filesystem::path rootPathObj(m_filePath);
    if(std::filesystem::exists(rootPathObj)) {
        std::filesystem::remove(rootPathObj);
    }
}

}
}
