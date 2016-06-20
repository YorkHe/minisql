#include "RecordManager.h"
#include "utility.h"
#include <iostream>

/*

CError RecordManager::selectRecord(Condition cond)
{
	//select_from_buffer(cond);
}

CError RecordManager::deleteRecord(Condition cond)
{
	//select_from_buffer(cond);
}

CError RecordManager::insertRecord(Tuple tuple)
{

}

*/

void RecordManager::printAll(std::ostream &out)
{
	for (auto val:this->result)
	{
		out << "[" << val.attr_name << "]" << val.value << std::endl;
	}

}

void RecordManager::printLimit(std::ostream &out, int from, int count)
{
	for (int i = from; i < from + count; i++) {
		out << "[" << this->result[i].attr_name  << "]" << this->result[i].value << std::endl;
	}
}

int main()
{
	return 0;
}