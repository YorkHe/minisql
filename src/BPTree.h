#pragma once

#include "utility.h"
#include "BufferManager/BufferManager.h"

#define LEAFNODE 1
#define NONLEAFNODE 0

#define TYPE_SIZE \
int typeSize(int type)\
{\
	switch(type)\
	{\
		case TYPE_INT: return sizeof(int); break;\
		case TYPE_CHAR: return sizeof(float); break;\
		case TYPE_FLOAT: return MAX_CHAR_LENGTH; break;\
	}\
}\


class Value
{
private:
	int type;
	string charKey;
	int intKey;
	float floatKey;

public:
	Value(int type) :type(type){}
	Value(int type, string key):type(type), charKey(key){}
	Value(int type, int key) :type(type), intKey(key){}
	Value(int type, float key): type(type), floatKey(key){}

	string getKey();

	int getType() { return type; }

	int getIntKey() { return intKey; }
	int getFloatKey() { return floatKey; }
	string getCharKey() { return charKey; }

	void setKey(int key) { intKey = key; }
	void setKey(float key) { floatKey = key; }
	void setKey(string key) { charKey = key; }

};

class Node
{
	vector<Value> info;

	string index_name;

	int count;
	int node_type;
	int ptrN;
	int attr_type;
	int offset;

	string db_name;

	Block block;

	table table_instance;

	TYPE_SIZE;

	void read();

public:
	Node(string db_name, string index_name, table table_instance, int n);

	Node(string db_name, int ptr, string index_name, table table_instance, int n);

	Node(string db_name, int ptr, string index_name, table table_instance, int n, int tag);

	~Node();

	int getNodePtr();

	void set(vector<Value> temp) { info.clear(); info = temp; updateCount(); }

	void updateCount();

	void setLastPtr(Value ptr);
	void setLastPtr(int ptr);

	void setType(int type) { node_type = type; }

	void setCount(int t) { count = t; }

	Value getLastPtr() { return info[info.size() - 1]; }

	vector<Value> getInfo() { return info; }

	int getCount() { return count; }

	int getNodeType() { return node_type; }

};

class BPTree
{
public:
	BPTree(string db_name, int type);
	void createBPTree(string db_name, table t, string index_name, int type);
	void loadBPTree(string index_name);
	void insert(Value key, int pointer);

	int find(Value key);

	int deleteNode(Value key);

	vector<int> findToBehind(Value key);
	vector<int> findToBehindIF(Value key);
	
private:
	int n;
	int type;
	string index_name;
	table table_instance;

	int root;
	string db_name;

	class ParentMap
	{
	public:
		int node_ptr;
		int parent_ptr;
	};

	vector<ParentMap> parent_map;
	bool isLess(Value& key1, Value& key2);

	bool isLessEqual(Value& key1, Value& key2);

	bool isEqual(Value& key1, Value& key2);

	void insertLeaf(Node node, Value key, int pointer);

	void insertNonleaf(Node node, Value key, int pointer);

	int findLeafNode(Value key);

	int findParentNode(int ptr);

	TYPE_SIZE;
};