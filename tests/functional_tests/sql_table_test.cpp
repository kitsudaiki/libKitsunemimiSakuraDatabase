#include "sql_table_test.h"

#include <libKitsunemimiSakuraDatabase/sql_database.h>
#include <libKitsunemimiSakuraDatabase/sql_table.h>

#include <libKitsunemimiJson/json_item.h>

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
    ErrorContainer error;

    Kitsunemimi::Json::JsonItem testData;
    testData.insert("name", "user0815");
    testData.insert("pw_hash", "secret");
    testData.insert("is_admin", true);

    m_uuid = "user0815";
    TEST_EQUAL(m_table->addUser(testData, error), true);


    Kitsunemimi::Json::JsonItem testData2;
    testData2.insert("name", "another user");
    testData2.insert("pw_hash", "secret2");
    testData2.insert("is_admin", false);

    m_table->addUser(testData2, error);
}

/**
 * @brief get_test
 */
void
SqlTable_Test::get_test()
{
    Kitsunemimi::Json::JsonItem resultItem;
    TableItem resultTable;
    ErrorContainer error;

    TEST_EQUAL(m_table->getUser(resultItem, m_uuid, error), true);
    TEST_EQUAL(resultItem.toString(), std::string("{\"is_admin\":true,\"name\":\"user0815\"}"));

    TEST_EQUAL(m_table->getUser(resultTable, m_uuid, error), true);
    std::string compare = "+----------+----------+\n"
                          "| name     | is_admin |\n"
                          "+==========+==========+\n"
                          "| user0815 | true     |\n"
                          "+----------+----------+\n";
    TEST_EQUAL(resultTable.toString(), compare);
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
    TEST_EQUAL(result.getNumberOfColums(), 2);

    result.clearTable();
    TEST_EQUAL(m_table->getAllUser(result, error, true), true);
    TEST_EQUAL(result.getNumberOfRows(), 2);
    TEST_EQUAL(result.getNumberOfColums(), 3);
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
