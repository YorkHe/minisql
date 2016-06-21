#include "utility.h"
#include <vector>
#include <ostream>
#include <BufferManager/block.h>

class RecordManager{
public:
	RecordManager();
	CError writeBlock(Block& blocks, int j, int tuple_length, vector<Attr>& attrList, vector<string>& values);
	void getOneTuple(Block& blocks, int j, int tuple_length, vector<Attr>& attr_list, vector<string>& tuple);
	CError selectRecord(Condition cond);

	CError deleteRecord(Condition cond);

	CError insertRecord(Tuple tuple);

	void printAll(std::ostream &out);

	void printLimit(std::ostream &out, int from, int count);
private:
	std::vector<Element> result;
};
