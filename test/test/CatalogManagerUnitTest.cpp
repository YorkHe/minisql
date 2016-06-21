#include "stdafx.h"

BOOST_AUTO_TEST_SUITE(CatalogManagerTest)

namespace fs = boost::filesystem;

std::string db_name("db");
std::string table_name("table");

fs::path db_path(DATABASE_FOLDER + db_name);
fs::path table_path(TABLE_PATH(db_name, table_name));


vector<Attr> attrs;
Attr attr(std::string("id"), TYPE_INT, sizeof(int));


BOOST_AUTO_TEST_CASE( LinkerUnitTest )
{
	CatalogManager cm;
	BOOST_TEST(cm.testLinker());
}

BOOST_AUTO_TEST_CASE(CatalogManager_CreateDatabase)
{
	CatalogManager cm;
	

	fs::path db_file_path(DATABASE_FOLDER + db_name + ("/" + db_name + ".db"));

	cm.createDatabase(db_name);

	BOOST_CHECK(fs::exists(db_path));
	BOOST_CHECK(fs::exists(db_file_path));

	fs::remove_all(db_path);
}

BOOST_AUTO_TEST_CASE(CatalogManager_CreateTable)
{
	CatalogManager cm;
	
	/*
	CError res = cm.createTable("db", "table", attrs);
	BOOST_CHECK(res.err_code == ERR_DB_NOT_EXISTS);
	*/
	cm.createDatabase("db");
	attrs.empty();
	attrs.push_back(attr);

	CError res2 = cm.createTable("db", "table", attrs);

	
	
	fs::path db_path(DATABASE_FOLDER + std::string("db"));
	fs::path table_path(DATABASE_FOLDER + std::string("db/table/table.tabconf"));
	fs::path tabledata_path(DATABASE_FOLDER + std::string("db/table/table.data"));
	
	BOOST_CHECK(fs::exists(table_path));
	BOOST_CHECK(fs::exists(tabledata_path));

	cm.dropDatabase(db_name);
	//fs::remove_all(db_path);

}

BOOST_AUTO_TEST_CASE(CatalogManager_dropDataBase)
{
	CatalogManager cm;

	cm.createDatabase(db_name);

	BOOST_CHECK(fs::exists(db_path));

	cm.dropDatabase(db_name);

	BOOST_CHECK(!fs::exists(db_path));

	//fs::remove_all(db_path);
}


BOOST_AUTO_TEST_CASE(CatalogManager_DropTable)
{
	CatalogManager cm;
	cm.createDatabase(db_name);

	attrs.empty();
	attrs.push_back(attr);
	cm.createTable(db_name, table_name, attrs);
	BOOST_CHECK(fs::exists(table_path));
	cm.dropTable(db_name, table_name);
	BOOST_CHECK(!fs::exists(table_path));

	cm.dropDatabase(db_name);
}

BOOST_AUTO_TEST_CASE(CatalogManager_EmptyTable)
{
	CatalogManager cm;
	cm.createDatabase(db_name);

	attrs.empty();
	attrs.push_back(attr);
	cm.createTable(db_name, table_name, attrs);

	int errcode = cm.emptyTable(db_name, table_name).err_code;
	BOOST_TEST_MESSAGE(errcode);
	BOOST_TEST(errcode == ERR_SUCCESS);
	
	cm.dropDatabase(db_name);
}




BOOST_AUTO_TEST_SUITE_END()