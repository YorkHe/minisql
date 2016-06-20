#pragma once

#include <string>


#define DB_PATH(DB_NAME) DATABASE_FOLDER + DB_NAME

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
#define ERR_REF_NOT_SAME_TYPE 9

class CError
{

public:
	CError(int err_code, std::string description)
	{
		this->err_code = err_code;
		this->description = description;
	}

	int err_code;
	std::string description;
};

typedef struct _Attr {
	struct _Attr(std::string n, int t, bool p, bool r):name(n),type_id(t),is_prime(p),is_reference(r), db_ref(""), tab_ref(""), attr_ref(""){}
	std::string name;
	int type_id;
	bool indexed;
	bool is_prime;
	bool is_reference;	
	std::string db_ref;
	std::string tab_ref;
	std::string attr_ref;
}Attr;

//TODO: TBD
typedef struct _Formula {
	int a;
}Formula;

typedef struct _Condition {
	bool with_where;
	Formula* where_clause;
}Condition;

enum RELATION {
	EQUAL,
	GREATER,
	GREATER_AND_EQUAL,
	LESS,
	LESS_AND_EQUAL
};

#define TYPE_INT 0
#define TYPE_VARCHAR 1
#define TYPE_DATES 2
#define TYPE_NULL 3

typedef struct _Element {
	std::string attr_name;
	std::string value;
}Element;

struct Tuple {
	Element* element;
};

struct Index {

};

#define DATABASE_FOLDER "D:\\db\\"