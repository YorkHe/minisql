#include "utility.h"
#include <vector>
#include <ostream>
#include <BufferManager/block.h>

class RecordManager{
public:
	RecordManager(string db_name, string table_name)
	{
		Table.dbname = db_name;
		Table.name = table_name;
	}
	RecordManager(table t) :Table(t) {};
	RecordManager(){};
	void push(Row& tuple, Results& results, vector<int> col_name_pos);
	template <class T>
	bool testCondition(T value1, T value2, int cond_condition);
	bool checkConditions(Row& tuple, vector<Attr>& attr_list, Condition conditions);
	CError writeBlock(Block& blocks, int j, int tuple_length, vector<Attr>& attrList, vector<string>& values);
	void getOneTuple(Block& blocks, int j, int tuple_length, vector<Attr>& attr_list, Row& tuple);
	CError selectRecord(vector<string> attr_name, Condition cond);
	CError selectRecord(Condition cond);

	CError deleteRecord(Condition cond);

	CError insertRecord(Tuple tuple);

	void printAll(std::ostream &out);

	void printLimit(std::ostream &out, int from, int count);
private:
	table Table;
	std::vector<Element> result;
};
