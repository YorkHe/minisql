#include "CatalogManger.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>


namespace fs = boost::filesystem;

CError CatalogManager::createDatabase(string DB_name)
{
	fs::path db_path(DB_PATH(DB_name));

	if (!fs::exists(db_path))
	{
		fs::create_directory(db_path);
		ofstream db_file;		
		db_file.open(DB_PATH(DB_name) + ("/" + DB_name+".db"));
		db_file << DB_name << endl;
		db_file.close();

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
	fs::path table_path(TABLE_PATH(DB_name, table_name));
	return fs::exists(table_path);
}

CError CatalogManager::createTable(string DB_name, string table_name, vector<Attr> attrs)
{
	if (!checkDatabaseExists(DB_name))
	{
		return CError(ERR_DB_NOT_EXISTS, "Database " + DB_name + "not exists");
	}

	fs::path table_path(TABLE_PATH(DB_name, table_name));

	if (!fs::exists(table_path))
	{
		fs::create_directory(table_path);

		ofstream table_file;
		table_file.open(TABLE_PATH(DB_name, table_name) +  "/" + (table_name + ".tabconf"));
		table_file << table_name << endl;		
		
		for (auto attr : attrs)
		{
			table_file << attr.name << " " << attr.type_id << endl;
		}
		table_file.close();
		
		ofstream table_data_file;
		table_data_file.open(TABLE_PATH(DB_name, table_name) + "/" + (table_name + ".data"));
		table_data_file.close();

		ofstream db_file;
		db_file.open(DB_PATH(DB_name) +  "/" + (DB_name + ".db"), ios::app);
		db_file << table_name << endl;
		db_file.close();
		return CError(ERR_SUCCESS, "");
	}
	else {
		return CError(ERR_TAB_ALREADY_EXISTS, "Table Already Exists.");
	}
}

CError CatalogManager::createTable(string table_name, vector<Attr> attrs)
{
	return createTable(DB, table_name, attrs);
}


CError CatalogManager::createIndex(string DB_name, string table_name, string attr_name)
{
	fs::path index_path(TABLE_PATH(DB_name, table_name)+"/"+attr_name+".index");
	loadAttr(DB_name, table_name);

	if (!fs::exists(index_path))
	{
		ofstream index_file;
		index_file.open(index_path.c_str());
		//TODO: Create index into the file.
		index_file.close();

		m_attrs[attr_name].indexed = true;

		flushAttr(DB_name, table_name);

		return (CError(ERR_SUCCESS, ""));
	}
	else {
		return (CError(ERR_IDX_EXISTS, "Index Already Exists."));
	}

}

CError CatalogManager::createIndex(string table_name, string attr_name)
{
	return createIndex(DB, table_name, attr_name);
}

CError CatalogManager::dropDatabase(string DB_name)
{
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
	fs::path table_path(TABLE_PATH(DB_name, table_name));

	if (!fs::exists(table_path))
	{
		return CError(ERR_TAB_NOT_EXISTS, "Table not exists");
	}

	boost::system::error_code ec;
	if (fs::remove_all(table_path, ec))
	{
		return CError(ERR_SUCCESS, "");
	}
	else {
		return CError(ERR_TAB_UNKNOWN, ec.message());
	}	                                                                   

}

CError CatalogManager::dropTable(string table_name)
{
	return dropTable(DB, table_name);
}


CError CatalogManager::dropIndex(string DB_name, string table_name, string attr_name)
{
	boost::system::error_code ec;
	fs::path index_path(TABLE_PATH(DB_name, table_name) + "/" + attr_name + ".index");

	if (fs::exists(index_path))
	{
		fs::remove(index_path);
		return CError(ERR_SUCCESS, "");
	}
	else {
		return CError(ERR_IDX_NOT_EXISTS, "Index Not Exists");
	}
}

CError CatalogManager::dropIndex(string table_name, string attr_name)
{
	return dropIndex(DB, table_name, attr_name);
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

CError CatalogManager::emptyTable(string table_name)
{
	return emptyTable(DB, table_name);
}



map<string, Attr> CatalogManager::loadAttr(string DB_name, string table_name)
{
	if (cur_DB == DB_name && cur_table == table_name)
	{
		return m_attrs;
	}

	cur_DB = DB_name;
	cur_table = table_name;

	map <string, Attr> ret;
	ifstream attr_file;
	attr_file.open(TABLE_PATH(DB_name, table_name) + "/" + (table_name + ".tabconf"), ios::in);

	string table_n;
	string name;
	int type;
	int prime, ref;

	attr_file >> table_n;

	while (attr_file >> name >> type >> prime >> ref)
	{
		ret[name] = Attr(name, type, prime, ref);
	}

	m_attrs = ret;

	return ret;
}

vector<string> CatalogManager::getAllAttributes(string DB_name, string table_name)
{
	vector<string> res;
	char s[256];

	loadAttr(DB_name, table_name);

	for (auto i : m_attrs)
	{
		res.push_back(i.first);
	}
	
	return res;
}

vector<string> CatalogManager::getAllAttributes(string table_name)
{
	return getAllAttributes(DB, table_name);
}


vector<Attr> CatalogManager::getAllAttributesDetail(string DB_name, string table_name)
{
	loadAttr(DB_name, table_name);
	vector<Attr> ret;

	for (auto i : m_attrs)
	{
		ret.push_back(i.second);
	}
	return ret;
}

vector<Attr> CatalogManager::getAllAttributesDetail(string table_name)
{
	return getAllAttributesDetail(DB, table_name);
}


Attr CatalogManager::getAttributeDetail(string DB_name, string table_name, string attr_name)
{
	loadAttr(DB_name, table_name);
	
	return m_attrs[attr_name];
}

Attr CatalogManager::getAttributeDetail(string table_name, string attr_name)
{
	return getAttributeDetail(DB, table_name, attr_name);
}

CError CatalogManager::flushAttr(string DB_name, string table_name)
{
	ofstream table_file;
	table_file.open(TABLE_PATH(DB_name, table_name) + "/" + (table_name + ".tabconf"));
	table_file << table_name << endl;

	for (auto iter : m_attrs)
	{
		auto attr = iter.second;
		table_file << attr.name << " " << attr.type_id << endl;
	}
	table_file.close();
}


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

vector<string> CatalogManager::getAllIndexes(string table_name)
{
	return getAllIndexes(DB, table_name);
}

CError CatalogManager::denotePrimeKey(string DB_name, string table_name, string attr_name)
{
	loadAttr(DB_name, table_name);

	m_attrs[attr_name].is_prime = true;
	//TODO: check if the attr is eligible as a prime key.
	

	flushAttr(DB_name, table_name);
}

CError CatalogManager::denotePrimeKey(string table_name, string attr_name)
{
	return denotePrimeKey(DB, table_name, attr_name);
}


CError CatalogManager::denoteForeignKey(string DB_name, string table_name, string attr_name, string DB_name_ref, string table_name_ref, string attr_name_ref)
{
	
	
	auto cur_attr = loadAttr(DB_name, table_name)[attr_name];
	auto ref_attr = loadAttr(DB_name_ref, table_name_ref)[attr_name_ref];

	
	if (cur_attr.type_id != ref_attr.type_id)
	{
		throw CError(ERR_REF_NOT_SAME_TYPE, "The two referenced key are not of the same type");
		return;
	}

	//TODO: check if the attr is eligible as foreign key. 

	
	flushAttr(DB_name, table_name);
}

CError CatalogManager::denoteForeignKey(string table_name, string attr_name, string DB_name_ref, string table_name_ref, string attr_name_ref)
{
	return denoteForeignKey(DB, table_name, attr_name, DB_name_ref, table_name_ref, attr_name_ref);
}


CError CatalogManager::useDatabase(string DB_name)
{
	this->DB = DB_name;
}

