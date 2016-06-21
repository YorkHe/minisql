#include "../utility.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class CatalogManager {
public:
	void writeHead(fstream& f, int& t_num);
	void readHead(fstream& f, int& t_num);
	void readTable(fstream& f, table& t);
	void writeTable(fstream& f, table& t);
	void readAttr(fstream& f, Attr& tmp_attr);
	void writeAttr(fstream& f, Attr& tmp_attr);
	CError createDatabase(string DB_name);
	CError createTable(string DB_name, string table_name, vector<Attr> attrs);
	CError createIndex(string DB_name, string table_name, string attr_name, table& target_table);

	CError dropDatabase(string DB_name);

	CError dropTable(string DB_name, string table_name);
	CError dropIndex(string DB_name, string table_name, string attr_name, table& target_table);

	CError emptyTable(string DB_name, string table_name);


	CError useDatabase(string DB_name);

	bool testLinker();

private:
	bool checkDatabaseExists(string DB_name);
	bool checkTableExists(string DB_name, string table_name);

	map<string, Attr> m_attrs;
	string cur_DB;
	string DB;
	string cur_table;
};