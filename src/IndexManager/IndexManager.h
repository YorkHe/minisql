#pragma once

#include "utility.h"
#include "BPTree.h"

class IndexManager
{
private:
	string db_name;
public:
	IndexManager(string db_name);
	
	void createIndex(string index_name);

	void createIndexOn(table table_instance, string index_name);

	bool dropIndex(table t, string index_name);

	bool dropIndexAll(table t);

	vector <int> selectOne(table t, string index_name, string key);
	vector <int> selectManyLess(table t, string index_name, string key);
	vector <int> selectManyLarger(table t, string index_name, string key);
	vector <int> selectManyLessE(table t, string index_name, string key);
	vector <int> selectManyLargerE(table t, string index_name, string key);

	void insertRec(table t, vector<string> index_list, string key, int block_ptr, int in_block_ptr);

	vector<int> deleteRec(table t, vector<string> index_list, string key);
};

