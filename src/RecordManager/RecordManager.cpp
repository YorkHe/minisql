#include "RecordManager.h"
#include "BufferManager/BufferManager.h"
#include "utility.h"
#include <iostream>
#include <sstream>

CError RecordManager::writeBlock(Block& blocks, int j, int tuple_length, vector<Attr> &attrList, vector<string> &values)
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
		if(attrList[k].type_id == TYPE_INT)
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

void RecordManager::getOneTuple(Block& blocks, int j, int tuple_length, vector<Attr> &attr_list, vector<string> &tuple)
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
			tuple.push_back(ss.str());
		}

		//int
		if (attr_list[k].type_id == TYPE_INT)
		{
			memcpy(&int_num, blocks.content + j * tuple_length + p, sizeof(int));
			p += sizeof(int);
			ss << int_num;
			tuple.push_back(ss.str());
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
			tuple.push_back(str);
		}
	}
}

CError RecordManager::selectRecord(Condition cond)
{

}

CError RecordManager::deleteRecord(Condition cond)
{

}

CError RecordManager::insertRecord(Tuple tuple)
{

}

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