#include "RecordManager.h"
#include "BufferManager/BufferManager.h"
#include "CatalogManger/CatalogManger.h"
#include "utility.h"
#include <iostream>
#include <sstream>
#include <map>

void RecordManager::push(Row& tuple, Results& results, vector<int> col_name_pos)
{
	Row row;
	for (auto col:col_name_pos)
	{
		row.col.push_back(tuple.col[col]);
	}
	results.row.push_back(row);
}

template <class T>
bool RecordManager::testCondition(T value1, T value2, int cond_condition)
{
	switch (cond_condition)
	{
	case NOT_EQUAL:
		if (value1 != value2)
			return true;
		break;
	case EQUAL:
		if (value1 == value2)
			return true;
		break;
	case LESS_AND_EQUAL:
		if (value1 <= value2)
			return true;
		break;
	case GREATER_AND_EQUAL:
		if (value1 >= value2)
			return true;
		break;
	case LESS:
		if (value1 < value2)
			return true;
		break;
	case GREATER:
		if (value1 > value2)
			return true;
		break;
	}
	return false;
}

bool RecordManager::checkConditions(Row& tuple, vector<Attr>& attr_list, Condition conditions)
{
	string cond_left, cond_right;
	int cond_condition;
	int type;
	int pos;
	for (auto condition: conditions.where_clause)
	{
		cond_left = condition.left;
		cond_right = condition.right;
		cond_condition = condition.relation;


		for (int i = 0; i < attr_list.size(); ++i)
		{
			if (attr_list[i].name == cond_left)
			{
				pos = i;
				break;
			}
		}

		type = attr_list[pos].type_id;

		switch (type)
		{
		case TYPE_INT:
			{
				int value1 = atoi(tuple.col[pos].c_str());
				int value2 = atoi(cond_right.c_str());
				return testCondition<int>(value1, value2, cond_condition);
			}
		case TYPE_FLOAT:
			{
				float value1 = atof(tuple.col[pos].c_str());
				float value2 = atof(cond_right.c_str());
				return testCondition<float>(value1, value2, cond_condition);
			}
		case TYPE_CHAR:
			{
				string value1 = tuple.col[pos];
				string value2 = cond_right;
				return testCondition<string>(value1, value2, cond_condition);
			}
		}
	}
}

CError RecordManager::writeBlock(Block& blocks, int j, int tuple_length, vector<Attr>& attrList, vector<string>& values)
{
	int p = 1;
	float float_num;
	int int_num;
	string str;

	for (int k = 0; k < attrList.size(); k++)
	{
		//float
		if (attrList[k].type_id == TYPE_FLOAT)
		{
			float_num = atof(values[k].c_str());
			memcpy(blocks.content + j * tuple_length + p, &float_num, sizeof(float));
			p += sizeof(float);
		}


		//int
		if (attrList[k].type_id == TYPE_INT)
		{
			int_num = atof(values[k].c_str());
			memcpy(blocks.content + j * tuple_length + p, &int_num, sizeof(int));
			p += sizeof(int);
		}

		//string
		if (attrList[k].type_id == TYPE_CHAR)
		{
			str = values[k];

			if ((str.length()) > attrList[k].length)
			{
				throw CError(ERR_ATTR_FORMAT_ERROR, "Insertion Failed.");
			}

			if ((str.length()) < attrList[k].length)
			{
				int diff = attrList[k].length - str.length();
				for (int i = 0; i < diff; i++)
				{
					str.insert(0, "\0");
				}
			}

			memcpy(blocks.content + j * tuple_length + p, str.c_str(), attrList[k].length + 1);
			p += attrList[k].length + 1;
		}
	}

	blocks.content[j * tuple_length] = 1;
	blocks.isDirty = true;

	return CError(ERR_SUCCESS, "");
}

void RecordManager::getOneTuple(Block& blocks, int j, int tuple_length, vector<Attr>& attr_list, Row& tuple)
{
	int p = 1;

	float float_num;
	int int_num;

	string str;
	char ch[255];

	stringstream ss;

	for (int k = 0; k < attr_list.size(); k++)
	{
		//float
		if (attr_list[k].type_id == TYPE_FLOAT)
		{
			memcpy(&float_num, blocks.content + j * tuple_length + p, sizeof(float));
			p += sizeof(float);
			ss << float_num;
			tuple.col.push_back(ss.str());
		}

		//int
		if (attr_list[k].type_id == TYPE_INT)
		{
			memcpy(&int_num, blocks.content + j * tuple_length + p, sizeof(int));
			p += sizeof(int);
			ss << int_num;
			tuple.col.push_back(ss.str());
		}

		//string
		if (attr_list[k].type_id == TYPE_CHAR)
		{
			memcpy(ch, blocks.content + j * tuple_length + p, attr_list[k].length + 1);
			p = p + attr_list[k].length + 1;

			str = "";

			int ii = 0;
			while (ch[ii] == '\0')
			{
				ii++;
				if (ii >= attr_list[k].length) break;
			}

			for (int j = ii; j < attr_list[k].length; j++)
			{
				str = str + ch[j];
			}

			str = str + '\0';
			tuple.col.push_back(str);
		}
	}
}

CError RecordManager::selectRecord(vector<string> attr_name, Condition cond)
{
	long num = 0;
	Row row;
	Results results;
	bool indexed = false;


	BufferManager BM(Table.dbname);

	string table_name = Table.name;

	vector<Attr> attr_list = Table.attr_list;

	vector<int> attr_name_pos;
	vector<int> block_vector = BM.getTableBlocks(table_name);
	vector<string> index_attr;

	int block_len;
	int tuple_len = Table.rec_length + 1;

	Row one_tuple;



	if (attr_name[0] == "*")
	{
		attr_name.clear();
		for (auto attr: attr_list)
		{
			attr_name.push_back(attr.name);
		}
	}

	attr_name_pos.clear();

	for (int i = 0; i < cond.where_clause.size(); i++)
	{
		string attr = cond.where_clause[i].left;
		for (int j = 0; j < attr_list.size(); j++)
		{
			if (attr == attr_list[j].name)
			{
				if (attr_list[j].indexed)
				{
					indexed = true;
					index_attr.push_back(attr);
				}
				attr_name_pos.push_back(j);
				break;
			}
		}
	}


	if (indexed)
	{
		int block_id;
		int record_id;

		block_len = 4096 / tuple_len;

		string first_index = index_attr[0];

		//TODO: INDEX

	}else
	{
		block_len = 4096 / tuple_len;
		for (int i = 0; i < block_vector.size(); i++)
		{
			Block& blocks = BM.getBlocks(i);
			for (int j = 0; j < block_len; j++)
			{
				if (blocks.content[j*tuple_len] == 1)
				{
					Row tuple;
					getOneTuple(blocks, j, tuple_len, attr_list, tuple);

					if (!cond.with_where)
					{
						push(tuple, results, attr_name_pos);
						num++;
					}else
					{
						if (checkConditions(tuple, attr_list, cond))
						{
							push(tuple, results, attr_name_pos);
							num++;
						}
					}
				}
			}
		}
	}

	return CError(ERR_SUCCESS, "");
}

CError RecordManager::deleteRecord(Condition cond)
{
	return CError(ERR_SUCCESS, "");
}

CError RecordManager::insertRecord(Tuple tuple)
{
	return CError(ERR_SUCCESS, "");
}

void RecordManager::printAll(std::ostream& out)
{
	for (auto val:this->result)
	{
		out << "[" << val.attr_name << "]" << val.value << std::endl;
	}
}

void RecordManager::printLimit(std::ostream& out, int from, int count)
{
	for (int i = from; i < from + count; i++)
	{
		out << "[" << this->result[i].attr_name << "]" << this->result[i].value << std::endl;
	}
}
