#include "BPTree.h"

Node::Node(string db_name, int ptr, string index_name, table table_instance, int n) :index_name(index_name), table_instance(table_instance), ptrN(n), offset(ptr)
{
	BufferManager BM(db_name);

	block = BM.getIndexOneBlock(index_name, ptr);

	Node::read();
}

Node::Node(string db_name, string index_name, table table_instance, int n) :index_name(index_name), table_instance(table_instance), ptrN(n)
{
	BufferManager BM(db_name);
	block = BM.newIndexBlock(index_name);
	offset = block.offset;
}

Node::Node(string db_name, int ptr, string index_name, table table_instance, int n, int tag) :index_name(index_name), table_instance(table_instance), ptrN(n), offset(ptr)
{
	BufferManager BM(db_name);

	block = BM.getIndexOneBlock(index_name, ptr);
}

int Node::getNodePtr()
{
	return offset;
}

Node::~Node()
{
	char cout_stream[5000];

	char tmp_nodetype;

	if (node_type == LEAFNODE)
		tmp_nodetype = '1';
	else
		tmp_nodetype = '0';

	memcpy(cout_stream, &tmp_nodetype, sizeof(char));
	memcpy(cout_stream + 1, &count, sizeof(int));


	int j = 0;
	for (auto i : info)
	{
		switch(i.getType())
		{
		case TYPE_INT:
		{
			int temp = i.getIntKey();
			memcpy(cout_stream + 1 + 4 + j, &temp, typeSize(TYPE_INT));
			j += typeSize(TYPE_INT);
			break;
		}
		case TYPE_FLOAT:
		{
			float temp = i.getFloatKey();
			memcpy(cout_stream + 1 + 4 + j, &temp, typeSize(TYPE_FLOAT));
			j += typeSize(TYPE_FLOAT);
			break;
		}
		case TYPE_CHAR:
		{
			string temp = i.getCharKey();
			memcpy(cout_stream + 1 + 4 + j, &temp, typeSize(TYPE_CHAR));
			j += typeSize(TYPE_CHAR);
			break;
		}
		}
	}

	BufferManager BM(db_name);

	BM.writeIndexData(index_name, offset, cout_stream, j + 5);
}

void Node::read()
{
	for (auto attr:table_instance.attr_list)
	{
		if (attr.name == index_name)
		{
			attr_type = attr.type_id;
		}
	}

	memcpy(&node_type, block.content, 1);
	memcpy(&count, block.content + 1, sizeof(int));

	for (int i  = 0; i < (ptrN - 1); i++)
	{
		float tmp_float = 0;
		int tmp_int = 0;
		string tmp_string = "";
		int tmp_ptr = 0;

		Value tmp_value(attr_type);

		memcpy(&tmp_ptr, block.content + 5 + i * (typeSize(attr_type) + sizeof(int)), typeSize(TYPE_INT));

		Value ptr_tmpvalue(TYPE_INT, tmp_ptr);
		info.push_back(ptr_tmpvalue);

		switch(attr_type)
		{
		case TYPE_FLOAT:
		{
			memcpy(&tmp_float, block.content + 5 + i * (typeSize(attr_type) + sizeof(int)), typeSize(TYPE_FLOAT));
			tmp_value.setKey(tmp_float);
			break;
		}
		case TYPE_INT:
		{
			memcpy(&tmp_int, block.content + 5 + i * (typeSize(attr_type) + sizeof(int)), typeSize(TYPE_INT));
			tmp_value.setKey(tmp_int);
			break;
		}
		case TYPE_CHAR:
		{
			memcpy(&tmp_string, block.content + 5 + i * (typeSize(attr_type) + sizeof(int)), typeSize(TYPE_CHAR));
			tmp_value.setKey(tmp_string);
			break;
		}
		}

		info.push_back(tmp_value);
	}

	int tmp_ptr = 0;
	memcpy(&tmp_ptr, block.content + 5 + (ptrN - 1) * (typeSize(attr_type) + sizeof(int)), typeSize(TYPE_INT));
	Value ptr_tmpvalue(TYPE_INT, tmp_ptr);
	info.push_back(ptr_tmpvalue);
}

void Node::setLastPtr(int ptr)
{
	Value tmp(TYPE_INT, ptr);
	info[info.size() - 1] = ptr;
}

void Node::setLastPtr(Value ptr)
{
	info[info.size() - 1] = ptr;
}

void Node::updateCount()
{
	count = (info.size() + 1) / 2;
}

BPTree::BPTree(string db_name, int type):type(type), db_name(db_name)
{
	int free = 4 * 1024 - sizeof(int) - 1 - sizeof(int);

	n = free / (sizeof(int) + typeSize(type));
}

void BPTree::createBPTree(string db_name, table t, string index_name, int type)
{
	int record_length = t.rec_length;

	BufferManager BM(db_name);
	Block block = BM.newIndexBlock(index_name);

	vector<int> block_num = BM.getIndexBlocks(index_name);

	Node node(db_name, block_num[0], index_name, t, n, 1);
	Node new_node(db_name, index_name, t, n);

	node.setType(NONLEAFNODE);
	node.setCount(1);

	vector<Value> tmp;

	tmp.clear();
	tmp.push_back(Value(TYPE_INT, new_node.getNodePtr()));

	node.set(tmp);

	new_node.setType(LEAFNODE);
	new_node.setCount(1);

	vector<Value> tmp2;
	tmp2.clear();

	tmp2.push_back(Value(TYPE_INT, -1));

	switch(type)
	{
	case TYPE_INT:
		tmp2.push_back(Value(TYPE_INT, -1));
		break;
	case TYPE_FLOAT:
		tmp2.push_back(Value(TYPE_FLOAT, -1));
		break;
	case TYPE_CHAR:
		tmp2.push_back(Value(TYPE_CHAR, '*'));
	}

	new_node.set(tmp2);
	new_node.setLastPtr(-1);
}


void BPTree::loadBPTree(string index_name)
{
	BufferManager BM(db_name);
	int first_block = BM.getIndexBlocks(index_name)[0];
	Node root_node(db_name, first_block, index_name, table_instance, n);
	root = root_node.getInfo()[0].getIntKey();
}

int BPTree::find(Value key)
{
	Node *node = new Node(db_name, root, index_name, table_instance, n);

	while (node->getNodeType() != LEAFNODE)
	{
		vector<Value> tmp = node->getInfo();
		int i = 1;
		for (i=  1; i < tmp.size(); i+=2)
		{
			if (isLess(key, tmp[i]))
				break;
		}
		if (i >= tmp.size())
		{
			delete node;
			Node* node = new Node(db_name, atoi(tmp[tmp.size() - 1].getKey().c_str()), index_name, table_instance, n);
		}else
		{
			delete node;
			Node* node = new Node(db_name, atoi(tmp[i - 1].getKey().c_str()), index_name, table_instance, n);
		}
	}

	vector <Value> tmp = node->getInfo();
	int i;
	for (i = 1; i < tmp.size(); i+=2)
	{
		if (isEqual(key, tmp[i]))
			break;
	}

	if (i >= tmp.size())
		return -1;
	else
		return tmp[i - 1].getIntKey();

	delete node;
}

int BPTree::findLeafNode(Value key)
{
	Node *node = new Node(db_name, root, index_name, table_instance, n);
	parent_map.clear();
	ParentMap *tmp_map = new ParentMap();

	tmp_map->node_ptr = root;
	tmp_map->parent_ptr = -1;

	parent_map.push_back(*tmp_map);

	int j = 0;

	while (node->getNodeType() != LEAFNODE)
	{
		vector<Value> tmp = node->getInfo();
		int i;
		for (i = 1; i < tmp.size(); i+=2)
		{
			if (isLess(key, tmp[i]))
				break;
		}

		if (i >= tmp.size())
		{
			delete tmp_map;
			ParentMap *tmp_map = new ParentMap();

			tmp_map->node_ptr = atoi(tmp[tmp.size() - 1].getKey().c_str());
			tmp_map->parent_ptr = node->getNodePtr();
			parent_map.push_back(*tmp_map);

			delete node;
			Node *node = new Node(db_name, atoi(tmp[tmp.size() - 1].getKey().c_str()), index_name, table_instance, n);
		}else
		{
			delete tmp_map;
			ParentMap *tmp_map = new ParentMap();
			tmp_map->node_ptr = atoi(tmp[tmp.size() - 1].getKey().c_str());
			tmp_map->parent_ptr = node->getNodePtr();
			parent_map.push_back(*tmp_map);
			delete node;

			parent_map[j].node_ptr = atoi(tmp[tmp.size() - 1].getKey().c_str());
			Node *node = new Node(db_name, atoi(tmp[i - 1].getKey().c_str()), index_name, table_instance, n);
		}
	}
	return node->getNodePtr();
}

int BPTree::findParentNode(int ptr)
{
	for (auto i:parent_map)
	{
		if (i.node_ptr == ptr)
			return i.parent_ptr;
	}
}

vector<int> BPTree::findToBehind(Value key)
{
	vector<int> result;
	Node *node = new Node(db_name, findLeafNode(key), index_name, table_instance, n);
	while (1)
	{
		vector<Value> info = node->getInfo();
		for (int i = 0; i < info.size() - 1; i+2)
		{
			if (isLess(key, info[i+1]))
			{
				result.push_back(info[i].getIntKey());
			}
		}

		if (node->getLastPtr().getIntKey() != -1)
		{
			delete node;
			node = new Node(db_name, node->getLastPtr().getIntKey(), index_name, table_instance, n);
		}
		else
			break;
	}
	return result;
}

vector<int> BPTree::findToBehindIF(Value key)
{
	vector<int> result;
	int j = -1;
	Node *node = NULL;
	BufferManager BM(db_name);
	while(1)
	{
		do
		{
			j++;
			node = new Node(db_name, BM.getIndexBlocks(index_name)[j], index_name, table_instance, n);
		} while (node->getNodeType() == NONLEAFNODE);

		vector<Value> info = node->getInfo();
		for(int i = 0; i < info.size() - 1;  i+=2)
		{
			if (isLess(info[i + 1], key))
				result.push_back(info[i].getIntKey());
		}

		if (node->getLastPtr().getIntKey() != -1)
			delete node;
		else
			break;
	}
	return result;
}

void BPTree::insert(Value key, int pointer)
{
	int node_ptr = findLeafNode(key);
	Node node(db_name, node_ptr, index_name, table_instance, n);

	if (node.getCount() < (n - 1))
		insertLeaf(node, key, pointer);
	else
	{
		vector<Value> key_list = node.getInfo();
		if (isLess(key, key_list[0]))
		{
			key_list.insert(key_list.begin(), key);
			Value tmp(TYPE_INT, pointer);
			key_list.insert(key_list.begin(), tmp);
		}else
		{
			for (int i = (key_list.size() - 1 - 1); i >= 0; i -= 2)
			{
				if (isLessEqual(key_list[i], key))
				{
					Value tmp_ptr(TYPE_INT, pointer);
					key_list.insert(key_list.begin() + i + 1, tmp_ptr);
					key_list.insert(key_list.begin() + i + 2, key);
					break;
				}
			}
		}

		Node newNode(db_name, index_name, table_instance, n);
		int new_nodeptr = newNode.getNodePtr();
		newNode.setLastPtr(node.getLastPtr());
		node.setLastPtr(new_nodeptr);

		int break_point = 0;
		if (n % 2 == 0)
			break_point = (n / 2) * 2;
		else
			break_point = ((n / 2) + 1) * 2;

		vector <Value> tmp(key_list.begin(), key_list.end() + break_point);
		node.set(tmp);
		vector <Value> tmp2(key_list.begin() + break_point, key_list.end());
		newNode.set(tmp2);

		insertNonleaf(node, tmp2[0], new_nodeptr);
	}
}

void BPTree::insertLeaf(Node node, Value key, int pointer)
{
	vector<Value> key_list = node.getInfo();
	if(isLess(key, key_list[0]))
	{
		key_list.insert(key_list.begin(), key);
		Value tmp(TYPE_INT, pointer);
		key_list.insert(key_list.begin(), tmp);
	}else
	{
		for (int i = (key_list.size() - 1 -1); i >= 0; i -= 2)
		{
			if (isLessEqual(key_list[i], key))
			{
				Value tmp_ptr(TYPE_INT, pointer);
				key_list.insert(key_list.begin() + i + 1, tmp_ptr);
				key_list.insert(key_list.begin() + i + 2, key);
				break;
			}
		}
	}

	node.set(key_list);
}

void BPTree::insertNonleaf(Node node, Value key, int pointer)
{
	BufferManager BM(db_name);
	if (node.getNodePtr() == root)
	{
		Node new_node(db_name, index_name, table_instance, n);
		vector<Value> key_list;
		Value tmp1(TYPE_INT, node.getNodePtr());
		Value tmp2(TYPE_INT, pointer);

		key_list.push_back(tmp1);
		key_list.push_back(key);
		key_list.push_back(tmp2);

		new_node.set(key_list);
		root = new_node.getNodePtr();

		int first_block = BM.getIndexBlocks(index_name)[0];
		Node root_node(db_name, first_block, index_name, table_instance, n);

		Value tmp3(TYPE_INT, root);
		vector<Value> tmp4;

		tmp4.push_back(tmp3);
		root_node.set(tmp4);
	}
	else
	{
		Node parent_node(db_name, findParentNode(node.getNodePtr()), index_name, table_instance, n);
		if (parent_node.getCount() < n)
		{
			vector<Value> key_list = parent_node.getInfo();
			if (isLess(key, key_list[0]))
			{
				Value tmp_ptr(TYPE_INT, pointer);
				key_list.insert(key_list.begin() + 1, tmp_ptr);
				key_list.insert(key_list.begin() + 1, key);
			}else
			{
				for (int i = (key_list.size() - 1 - 1); i >= 0; i -=2)
				{
					if(isLessEqual(key_list[i], key))
					{
						Value tmp_ptr(TYPE_INT, pointer);
						key_list.insert(key_list.begin() + i + 2, tmp_ptr);
						key_list.insert(key_list.begin() + i + 2, key);
						break;
					}
				}
			}
			parent_node.set(key_list);
		}else
		{
			vector<Value> key_list = parent_node.getInfo();
			if (isLess(key, key_list[0]))
			{
				Value tmp_ptr(TYPE_INT, pointer);
				key_list.insert(key_list.begin() + 1, tmp_ptr);
				key_list.insert(key_list.begin() + 1, key);
			}else
			{
				for (int i = (key_list.size() - 1 - 1); i >= 0; i-=2)
				{
					if (isLessEqual(key_list[i], key))
					{
						Value tmp_ptr(TYPE_INT, pointer);
						key_list.insert(key_list.begin() + i + 2, tmp_ptr);
						key_list.insert(key_list.begin() + i + 2, key);
						break;
					}
				}
			}

			int break_point = 0;
			if (n%2 == 0)
			{
				break_point = n / 2;
			}else
			{
				break_point = n / 2 + 1;
			}
			vector<Value> tmp;
			tmp.push_back(key_list[0]);

			int j = 1;

			for (int i = 1; i < break_point; i++)
			{
				tmp.push_back(key_list[j]);
				tmp.push_back(key_list[++j]);
			}
			parent_node.set(tmp);

			Value new_key = key_list[++j];
			vector<Value> tmp2;

			for (int i = j +1; i < key_list.size(); i++)
			{
				tmp2.push_back(key_list[i]);
			}

			Node new_node(db_name, index_name, table_instance, n);
			new_node.set(tmp2);

			insertNonleaf(parent_node, new_key, new_node.getNodePtr());
		}
	}
}

string Value::getKey()
{
	switch (type)
	{
	case TYPE_CHAR:
		return charKey;
	case TYPE_INT:
	{
		char tmp[260];
		sprintf(tmp, "%i", intKey);
		return tmp;
	}
	case TYPE_FLOAT:
	{
		char tmp[260];
		sprintf(tmp, "%i", floatKey);
		return tmp;
	}
	default:
		return "";
	}
}

int BPTree::deleteNode(Value key)
{
	int ptr = find(key);

	Node node(db_name, findLeafNode(key), index_name, table_instance, n);

	vector<Value> tmp = node.getInfo();
	int i;

	for (i = 1; i < tmp.size(); i+= 2)
	{
		if (isEqual(key, tmp[i]))
			break;
	}

	if (i >= tmp.size())
		return -1;
	else
	{
		int ttmp = tmp[i - 1].getIntKey();
		tmp[i - 1].setKey(-1);
		node.set(tmp);
		return ttmp;
	}

}


bool BPTree::isLess(Value& key1,Value& key2)
{
	switch(key1.getType())
	{
	case TYPE_INT:if (key1.getIntKey() < key2.getIntKey()) return true;else return false;break;
	case TYPE_FLOAT:if (key1.getFloatKey() < key2.getFloatKey()) return true;else return false;break;
	case TYPE_CHAR:if (key1.getCharKey() < key2.getCharKey()) return true;else return false;break;
	default:
		return false;
	}
}

bool BPTree::isLessEqual(Value& key1,Value& key2)
{
	switch(key1.getType())
	{
	case TYPE_INT:if (key1.getIntKey() <= key2.getIntKey()) return true;else return false;break;
	case TYPE_FLOAT:if (key1.getFloatKey() <= key2.getFloatKey()) return true;else return false;break;
	case TYPE_CHAR:if (key1.getCharKey() <= key2.getCharKey()) return true;else return false;break;
	default:
		return false;
	}
}

bool BPTree::isEqual(Value& key1, Value& key2)
{
	switch (key1.getType())
	{
	case TYPE_INT:if (key1.getIntKey() == key2.getIntKey()) return true; else return false; break;
	case TYPE_FLOAT:if (key1.getFloatKey() == key2.getFloatKey()) return true; else return false; break;
	case TYPE_CHAR:if (key1.getCharKey() == key2.getCharKey()) return true; else return false; break;
	default:
		return false;
	}
}


