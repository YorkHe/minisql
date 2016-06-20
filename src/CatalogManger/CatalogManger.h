#include "..\utility.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class CatalogManager {
public:
	CError createDatabase(string DB_name);
	CError createTable(string DB_name, string table_name, vector<Attr> attrs);
	CError createTable(string table_name, vector<Attr> attrs);

	CError createIndex(string DB_name, string table_name, string attr_name);
	CError createIndex(string table_name, string attr_name);

	CError dropDatabase(string DB_name);

	CError dropTable(string DB_name, string table_name);
	CError dropTable(string table_name);

	CError dropIndex(string DB_name, string table_name, string attr_name);
	CError dropIndex(string table_name, string attr_name);

	CError emptyTable(string DB_name, string table_name);
	CError emptyTable(string table_name);

	vector<string> getAllAttributes(string DB_name, string table_name);
	vector<string> getAllAttributes(string table_name);

	vector<Attr> getAllAttributesDetail(string DB_name, string table_name);
	vector<Attr> getAllAttributesDetail(string table_name);

	Attr getAttributeDetail(string DB_name, string table_name, string attr_name);
	Attr getAttributeDetail(string table_name, string attr_name);

	vector<string> getAllIndexes(string DB_name, string table_name);
	vector<string> getAllIndexes(string table_name);

	CError denotePrimeKey(string DB_name, string table_name, string attr_name);
	CError denotePrimeKey(string table_name, string attr_name);

	CError denoteForeignKey(string DB_name, string table_name, string attr_name, string DB_name_ref, string table_name_ref, string attr_name_ref);
	CError denoteForeignKey(string table_name, string attr_name, string DB_name_ref, string table_name_ref, string attr_name_ref);

	CError useDatabase(string DB_name);

	bool testLinker();

private:
	bool checkDatabaseExists(string DB_name);
	map<string, Attr> loadAttr(string DB_name, string table_name);

	CError flushAttr(string DB_name, string table_name);
	map<string, Attr> m_attrs;
	string cur_DB;
	string DB;
	string cur_table;
};