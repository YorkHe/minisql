#include "CatalogManger.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>


namespace fs = boost::filesystem;

void CatalogManager::writeHead(fstream &f, int& t_num)
{
	f.write(reinterpret_cast<char*>(&t_num), sizeof(int));
	f.flush();
}

void CatalogManager::readHead(fstream &f, int& t_num)
{
	f.read(reinterpret_cast<char*>(&t_num), sizeof(int));
}

void CatalogManager::readTable(fstream &f, table& t)
{
	char buf[MAX_CHAR_LENGTH];
	f.read(reinterpret_cast<char*>(buf), MAX_CHAR_LENGTH);
	t.name = buf;
	f.read(reinterpret_cast<char*>(buf), MAX_CHAR_LENGTH);
	t.dbname = buf;

	f.read(reinterpret_cast<char*>(&(t.attr_num)), sizeof(int));
	f.read(reinterpret_cast<char*>(&(t.rec_length)), sizeof(int));
	f.read(reinterpret_cast<char*>(&(t.rec_num)), sizeof(int));
	f.read(reinterpret_cast<char*>(&(t.size)), sizeof(int));

	Attr tmp_attr;
	for (int i = 0; i < t.attr_num; i++)
	{
		readAttr(f, tmp_attr);
		t.attr_list.push_back(tmp_attr);
	}
}

void CatalogManager::writeTable(fstream &f, table& t)
{
	f.write(t.name.c_str(), MAX_CHAR_LENGTH);
	f.write(t.dbname.c_str(), MAX_CHAR_LENGTH);
	f.write(reinterpret_cast<char*>(&t.attr_num), sizeof(int));
	f.write(reinterpret_cast<char*>(&t.rec_length), sizeof(int));
	f.write(reinterpret_cast<char*>(&t.rec_num), sizeof(int));
	f.write(reinterpret_cast<char*>(&t.size), sizeof(int));
	for (auto attr : t.attr_list)
		writeAttr(f, attr);
	f.flush();
}

void CatalogManager::readAttr(fstream &f, Attr &tmp_attr)
{
	char buf[MAX_CHAR_LENGTH];
	f.read(reinterpret_cast<char*>(buf), MAX_CHAR_LENGTH);
	f.read(reinterpret_cast<char*>(&tmp_attr.type_id), sizeof(int));
	f.read(reinterpret_cast<char*>(&tmp_attr.length), sizeof(int));
	f.read(reinterpret_cast<char*>(&tmp_attr.is_prime), sizeof(bool));
	f.read(reinterpret_cast<char*>(&tmp_attr.is_unique), sizeof(bool));
	f.read(reinterpret_cast<char*>(&tmp_attr.not_null), sizeof(bool));
	f.read(reinterpret_cast<char*>(&tmp_attr.indexed), sizeof(bool));
	tmp_attr.name = buf;
}

void CatalogManager::writeAttr(fstream &f, Attr &tmp_attr)
{
	f.write((tmp_attr.name.c_str()), MAX_CHAR_LENGTH);
	f.write(reinterpret_cast<char*>(&tmp_attr.type_id), sizeof(int));
	f.write(reinterpret_cast<char*>(&tmp_attr.length), sizeof(int));
	f.write(reinterpret_cast<char*>(&tmp_attr.is_prime), sizeof(bool));
	f.write(reinterpret_cast<char*>(&tmp_attr.is_unique), sizeof(bool));
	f.write(reinterpret_cast<char*>(&tmp_attr.not_null), sizeof(bool));
	f.write(reinterpret_cast<char*>(&tmp_attr.indexed), sizeof(bool));
}





CError CatalogManager::createDatabase(string DB_name)
{
	fs::path db_path(DB_PATH(DB_name));

	if (!fs::exists(db_path))
	{
		fs::create_directory(db_path);
		fstream fout;
		fout.open(DB_PATH(DB_name) + DB_name + ".list", ios::out | ios::binary);
		int t = 0;
		writeHead(fout, t);
		fout.close();

		return CError(ERR_SUCCESS, "");
	}
	else {
		return CError(ERR_DB_ALREADY_EXISTS, "Database Already Exists.");
	}
}

bool CatalogManager::checkDatabaseExists(string DB_name)
{
	fs::path db_path(DB_PATH(DB_name));
	return fs::exists(db_path);
}

bool CatalogManager::checkTableExists(string DB_name, string table_name)
{
	bool flag = false;
	fstream f;
	f.open(DB_PATH(DB_name) + DB_name + ".list", ios::in | ios::binary);

	int t_num;
	table t;

	readHead(f, t_num);

	for (int i = 0; i < t_num; i++)
	{
		readTable(f, t);
		if (t.name == table_name)
		{
			flag = true;
			break;
		}
	}

	f.close();
	return flag;
}

CError CatalogManager::createTable(string DB_name, string table_name, vector<Attr> attrs)
{
	table tmp_table;

	if (!checkDatabaseExists(DB_name))
	{
		throw CError(ERR_DB_NOT_EXISTS, "Database " + DB_name + "not exists");
	}

	if (checkTableExists(DB_name, table_name))
	{
		throw CError(ERR_TAB_ALREADY_EXISTS, "Table" + table_name + "already exists");
	}

	tmp_table.dbname = DB_name;
	tmp_table.name = table_name;
	tmp_table.rec_length = 0;
	tmp_table.rec_num = 0;
	tmp_table.size = MAX_CHAR_LENGTH * 2 + sizeof(int) * 4;

	for (auto attr : attrs)
	{
		Attr tmp_attr;
		tmp_attr.name = attr.name;
		tmp_table.size += MAX_CHAR_LENGTH + sizeof(int) * 2 + sizeof(bool) * 4;

		tmp_attr.type_id = attr.type_id;
		if (tmp_attr.type_id == TYPE_FLOAT)
		{
			tmp_table.rec_length += sizeof(float);
			tmp_attr.length = sizeof(float);
		}
		if (tmp_attr.type_id == TYPE_INT)
		{
			tmp_table.rec_length += sizeof(int);
			tmp_attr.length = sizeof(int);
		}
		if (tmp_attr.type_id == TYPE_CHAR)
		{
			tmp_table.rec_length += sizeof(char);
			tmp_attr.length = sizeof(char);
		}

		tmp_attr.is_prime = attr.is_prime;
		tmp_attr.is_unique = attr.is_unique;
		tmp_attr.not_null = attr.not_null;

		tmp_table.attr_list.push_back(tmp_attr);
	}

	tmp_table.attr_num = attrs.size();
	fstream f;
	f.open(DB_name + ".list", ios::in | ios::out | ios::beg);

	int t_num;

	f.seekg(0, ios::beg);
	readHead(f, t_num);
	t_num++;
	f.seekp(0, ios::beg);
	writeHead(f, t_num);
	f.seekp(0, ios::end);
	writeTable(f, tmp_table);
	f.close();

	return CError(ERR_SUCCESS, "");
}


CError CatalogManager::createIndex(string DB_name, string table_name, string attr_name, table &target_table)
{
	table tmp_table;
	if (!checkDatabaseExists(DB_name))
	{
		throw CError(ERR_DB_NOT_EXISTS, "Database Not Exists");
		return;
	}

	if(!checkTableExists(DB_name, table_name))
	{
		throw CError(ERR_TAB_NOT_EXISTS, "Table Not Exists");
		return;
	}

	fstream f;
	f.open(DB_FILE(DB_name) + ".list", ios::in | ios::out | ios::binary);

	int t_num;

	f.seekg(0, ios::beg);
	readHead(f, t_num);

	for (int i = 0; i<t_num; i++)
	{
		readTable(f, tmp_table);
		if (tmp_table.name == table_name)
			break;
	}

	bool flag = false;

	for(auto attr:tmp_table.attr_list)
	{
		if (attr.name == attr_name)
		{
			flag = true;
			if (attr.indexed)
			{
				throw CError(ERR_IDX_EXISTS, "Attribute " + attr_name + " already has index!");
			}
		}
	}

	if (!flag)
	{
		throw CError(ERR_ATTR_NOT_EXISTS, "Attribute " + attr_name + " doesn't exists");
	}

	target_table = tmp_table;
}

CError CatalogManager::dropDatabase(string DB_name)
{
	if (!checkDatabaseExists(DB_name))
	{
		return CError(ERR_DB_NOT_EXISTS, "Database Not Exists");
	}


	fs::path DB_path(DB_PATH(DB_name));
	boost::system::error_code ec;
	if (fs::remove_all(DB_path, ec))
	{
		return CError(ERR_SUCCESS, "");
	}
	else {
		return CError(ERR_DB_UNKNOWN, ec.message());
	}
}

bool CatalogManager::testLinker()
{
	return true;
}


CError CatalogManager::dropTable(string DB_name, string table_name)
{
	table tmp_table;
	if (!checkDatabaseExists(DB_name))
		throw CError(ERR_DB_NOT_EXISTS, "Database Not Exists");

	if (!checkTableExists(DB_name, table_name))
		throw CError(ERR_TAB_NOT_EXISTS, "Table Not Exists");

	fstream f;
	f.open(DB_name + ".list", ios::in | ios::out | ios::binary);
	int t_num;

	f.seekg(0, ios::beg);
	readHead(f, t_num);

	int readPos = f.tellg();
	int tt_num = t_num;
	t_num--;
	f.seekp(0, ios::beg);
	writeHead(f, t_num);

	int n = 0;
	int pos = 0;

	f.seekg(readPos);

	vector<table> tables;
	for (int i = 0; i < tt_num; i++)
	{
		readTable(f, tmp_table);
		tables.push_back(tmp_table);

		if (tmp_table.name == table_name)
		{
			n = i;
			pos = (int)f.tellg() - tmp_table.size;
		}
	}

	f.seekp(pos);
	for (int i = n + 1; i < tt_num; i++)
		writeTable(f, tables[i]);

	f.close();
	return CError(ERR_SUCCESS, "");
}


CError CatalogManager::dropIndex(string DB_name, string table_name, string attr_name, table &target_table)
{
	table tmp_table;
	if (!checkDatabaseExists(DB_name))
	{
		throw CError(ERR_DB_NOT_EXISTS, "Database not exists");
	}

	if (!checkTableExists(DB_name, table_name))
	{
		throw CError(ERR_TAB_NOT_EXISTS, "Table not exists");
	}

	fstream f;
	f.open(TABLE_PATH(DB_name, table_name) + "/" + (table_name + ".tabconf"), ios::in | ios::out | ios::binary);

	int t_num;
	f.seekg(0, ios::beg);
	readHead(f, t_num);

	for (int i = 0; i < t_num; i++)
	{
		readTable(f, tmp_table);
		if (tmp_table.name == table_name)
			break;
	}

	bool flag = false;

	for (auto attr:tmp_table.attr_list)
	{
		if (attr.name == attr_name)
		{
			flag = true;
			if (!attr.indexed)
			{
				throw CError(ERR_IDX_NOT_EXISTS, "Attribute " + attr_name + " Doesn't Have Index!");
			}
			break;
		}
	}

	if (!flag)
	{
		throw CError(ERR_ATTR_NOT_EXISTS, "Attribute " + attr_name + "Doesn't Exists");
	}

	target_table = tmp_table;

	return CError(ERR_SUCCESS, "");
}



CError CatalogManager::emptyTable(string DB_name, string table_name)
{
	if (!checkDatabaseExists(DB_name))
	{
		return CError(ERR_DB_NOT_EXISTS, "Database " + DB_name + "not exists");
	}

	fs::path table_data_path(TABLE_PATH(DB_name, table_name));

	if (!fs::exists(table_data_path))
	{
		ofstream table_data_file;
		table_data_file.open(TABLE_PATH(DB_name, table_name) + "/" + (table_name + ".data"));
		table_data_file.close();
	}
	else {
		return CError(ERR_TAB_NOT_EXISTS, "Table" + table_name + "not exists");
	}
}


/*
vector<string> CatalogManager::getAllIndexes(string DB_name, string table_name)
{
	vector<string> ret;
	loadAttr(DB_name, table_name);

	for (auto iter : m_attrs)
	{
		auto attr = iter.second;
		if (attr.indexed)
		{
			ret.push_back(attr.name);
		}
	}
	return ret;
}


CError CatalogManager::denotePrimeKey(string DB_name, string table_name, string attr_name)
{
	loadAttr(DB_name, table_name);

	m_attrs[attr_name].is_prime = true;
	

	flushAttr(DB_name, table_name);
}
*/


CError CatalogManager::useDatabase(string DB_name)
{
	this->DB = DB_name;
}

