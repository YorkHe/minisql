#include "IndexManager.h"

IndexManager::IndexManager(string db_name)
{
	db_name = db_name;
}

void IndexManager::createIndex(string index_name)
{
	
}


void IndexManager::createIndexOn(table table_instance, string index_name)
{
	if(table_instance.rec_num == 0)
	{
		createIndex(index_name);
		return;
	}

	int attr_type = 0;
	for (auto attr : table_instance.attr_list)
	{
		if (attr.name == index_name)
		{
			attr_type = attr.type_id;
			break;
		}
	}

	BPTree tree(db_name, attr_type);
	tree.createBPTree(db_name, table_instance, index_name, attr_type);
}

vector<int> IndexManager::selectOne(table t, string index_name, string key)
{
	vector<int> result;

	int attr_type = 0;

	for (auto attr: t.attr_list)
	{
		if (attr.name == index_name)
		{
			attr_type = attr.type_id;
			break;
		}
	}

	BPTree tree(db_name, attr_type);

	tree.loadBPTree(t.name + "." + index_name);
	result.push_back(tree.find(Value(attr_type, key)));

	return result;
}

vector<int> IndexManager::selectManyLarger(table t, string index_name, string key)
{
	vector<int> result;
	result.clear();

	int attr_type = 0;
	for (auto attr: t.attr_list)
	{
		if (attr.name == index_name)
		{
			attr_type = attr.type_id;
			break;
		}
	}

	BPTree tree(db_name, attr_type);
	tree.loadBPTree(t.name + "." + index_name);
	
	return tree.findToBehind(Value(attr_type, key));
}

vector<int> IndexManager::selectManyLess(table t, string index_name, string key)
{
	vector<int> result;
	result.clear();

	int attr_type = 0;
	for (auto attr: t.attr_list)
	{
		if (attr.name == index_name)
		{
			attr_type = attr.type_id;
			break;
		}
	}

	BPTree tree(db_name, attr_type);
	tree.loadBPTree(t.name + "." + index_name);
	
	return tree.findToBehindIF(Value(attr_type, key));
}

vector<int> IndexManager::selectManyLargerE(table t, string index_name, string key)
{
	vector<int> result, tmp_r;
	result.clear();

	int attr_type = 0;
	for (auto attr: t.attr_list)
	{
		if (attr.name == index_name)
		{
			attr_type = attr.type_id;
			break;
		}
	}

	BPTree tree(db_name, attr_type);
	tree.loadBPTree(t.name + "." + index_name);

	Value tmp(attr_type, key);

	result.push_back(tree.find(tmp));

	tmp_r = tree.findToBehind(tmp);

	for (auto r : tmp_r)
	{
		result.push_back(r);
	}

	return result;
}

vector<int> IndexManager::selectManyLessE(table t, string index_name, string key)
{
	vector<int> result, tmp_r;
	result.clear();

	int attr_type = 0;
	for (auto attr: t.attr_list)
	{
		if (attr.name == index_name)
		{
			attr_type = attr.type_id;
			break;
		}
	}

	BPTree tree(db_name, attr_type);
	tree.loadBPTree(t.name + "." + index_name);

	Value tmp(attr_type, key);


	tmp_r = tree.findToBehindIF(tmp);

	for (auto r : tmp_r)
	{
		result.push_back(r);
	}

	result.push_back(tree.find(tmp));

	return result;
}

void IndexManager::insertRec(table t, vector<string> index_list, string key, int block_ptr, int in_block_ptr)
{
	for (int j = 0; j < index_list.size(); j++)
	{
		int attr_type = 0;
		for (auto attr: t.attr_list)
		{
			if (attr.name == index_list[j])
			{
				attr_type = attr.type_id;
			}
		}

		BPTree tree(db_name, attr_type);
		tree.loadBPTree(t.name + "." + index_list[j]);
		Value tmp(attr_type, key);
		int rec = 4096 / (t.rec_length + 1);
		tree.insert(tmp, block_ptr * rec + in_block_ptr);
	}
}

vector<int> IndexManager::deleteRec(table t, vector<string> index_list, string key)
{
	vector<int> result;

	for (int j = 0; j < index_list.size(); j++)
	{
		int attr_type = 0;
		for (auto attr:t.attr_list)
		{
			if (attr.name == index_list[j])
				attr_type = attr.type_id;
		}

		BPTree tree(db_name, attr_type);
		tree.loadBPTree(t.name + "." + index_list[j]);
		Value tmp(attr_type, key);
		result.push_back(tree.deleteNode(tmp));
	}

	return result;
}








