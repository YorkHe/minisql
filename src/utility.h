#pragma once

#include <string>
#include <vector>

#define MAX_CHAR_LENGTH 32
#define FILE_PAGESIZE 4096

#define DB_PATH(DB_NAME) DATABASE_FOLDER + DB_NAME
#define DB_FILE(DB_NAME) DATABASE_FOLDER + DB_NAME + "/" + DB_NAME

#define TABLE_PATH(DB_NAME, TABLE_NAME) DATABASE_FOLDER	 + DB_NAME + "/" + TABLE_NAME

#define ERR_SUCCESS 0
#define ERR_DB_ALREADY_EXISTS 1 
#define ERR_DB_NOT_EXISTS 2
#define ERR_DB_UNKNOWN 3
#define ERR_TAB_ALREADY_EXISTS 4
#define ERR_TAB_NOT_EXISTS 5
#define ERR_TAB_UNKNOWN 6
#define ERR_IDX_EXISTS 7
#define ERR_IDX_NOT_EXISTS 8
#define ERR_ATTR_NOT_EXISTS 9
#define ERR_REF_NOT_SAME_TYPE 10 
#define ERR_ATTR_FORMAT_ERROR 11

class CError
{

public:
	CError()
	{
		this->err_code = 0;
		this->description = "";
	}
	CError(int err_code, std::string description)
	{
		this->err_code = err_code;
		this->description = description;
	}

	int err_code;
	std::string description;
};

class Attr{
public:
	Attr():indexed(false),is_prime(false), not_null(false), is_unique(false){}
	Attr(std::string name, int type_id, int length):name(name), type_id(type_id), length(length), indexed(false),is_prime(false), not_null(false), is_unique(false){}
	std::string name;
	int type_id;
	int length;
	bool indexed;
	bool is_prime;
	bool not_null;
	bool is_unique;
};


class Formula
{
public:
	enum RELATION relation;
	std::string left;
	std::string right;
};

typedef struct _Condition {
	bool with_where;
	std::vector<Formula> where_clause;
}Condition;

enum RELATION {
	EQUAL,
	GREATER,
	GREATER_AND_EQUAL,
	LESS,
	LESS_AND_EQUAL,
	NOT_EQUAL
};

#define TYPE_INT 0
#define TYPE_CHAR 1
#define TYPE_DATES 2
#define TYPE_FLOAT 3
#define TYPE_NULL 4

typedef struct _Element {
	std::string attr_name;
	std::string value;
}Element;

struct Tuple {
	Element* element;
};

struct Index {

};

class table
{
public:
	table(const table& T)
	{
		name = T.name;
		dbname = T.dbname;
		attr_num = T.attr_num;
		rec_length = T.rec_length;
		rec_num = T.rec_num;
		size = T.size;
	}

	table() {};
	std::string name;
	std::string dbname;
	int attr_num;
	int rec_length;
	int rec_num;
	int size;
	std::vector<Attr> attr_list;
};

class Row
{
public:
	std::vector<std::string> col;
};

class Results
{
public:
	std::vector<Row> row;
};

#define DATABASE_FOLDER "D:\\db\\"