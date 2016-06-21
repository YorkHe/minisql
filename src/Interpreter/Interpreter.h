#ifndef Interpreter_H
#define Interpreter_H
#include<string>
#include<string.h>

using namespace std;

//表的属性类
class table_column
{
public:
    string column_name;   //列（属性）的名字
    int column_type;      //属性的类型：1代表整型int，2代表字符型char，3代表单精度浮点型float
    int char_num;   //如果属性是字符型，记录字符的长度
    int Is_unique;  //是否定义unique约束，-1代表无，1代表有
};

//条件类
class CONDITION
{
public:
	string column;       //属性（列）名
    int col_type;     //属性（列）的类型，1代表整型，2代表字符型，3代表单精度浮点型
	string op;        //比较运算符
	int value1;          //整型的值
	string value2;     //字符串型的值
    float value3;        //单精度浮点数型的值
    string value;
};

//select类
class SQLselect
{
public:
	string table_name;  //表名
	CONDITION select_condition[32];  //选择的条件
	int condition_num;   //条件的个数
};

//delete类
class SQLdelete
{
public:
	string table_name;  //表名
	CONDITION delete_condition[32];  //选择的条件
	int condition_num;   //条件的个数
};

//insert类
class SQLinsert
{
public:
	string table_name;  //表名
	CONDITION Insert_values[32];//插入的值
	int Values_num;   //值的个数
};

//execfile类
class SQLexecfile
{
public:
	string filename;  //文件名
};

//create_table类
class SQLcreate_table
{
public:
	string table_name;  //表名
	int Attribute_num;  //属性个数
    string primary_key;   //做主键的属性名
    table_column attribute[32];
};

//create_index类
class SQLcreate_index
{
public:
	string table_name;  //表名
	string index_name;  //索引名
	string column_name;  //列名
};

//drop_table类
class SQLdrop_table
{
public:
	string table_name;  //删除表的名字
};

//drop_index类
class SQLdrop_index
{
public:
	string index_name;  //删除索引的名字
};

//create_database类
class SQLcreate_database
{
public:
    string database_name;
};

//drop_database类
class SQLdrop_database
{
public:
    string database_name;
};

//use_database类
class SQLuse_database
{
public:
    string database_name;
};

class statement
{
public:
	int num; //  语句编号,1-12代表如下，9代表quit，0代表语句输入错误
	SQLselect  statement1;   //选择SQL语句
    SQLdelete  statement2;   //删除SQL语句
    SQLinsert  statement3;   //插入SQL语句
    SQLexecfile  statement4; //执行SQL文件语句
    SQLcreate_table  statement5;  //创建表SQL语句
    SQLcreate_index  statement6;  //创建索引SQL语句
    SQLdrop_table  statement7;   //删除表SQL语句
    SQLdrop_index  statement8;   //删除索引SQL语句
    SQLcreate_database statement10;  //创建数据库SQL语句
    SQLdrop_database statement11;    //删除数据库SQL语句
    SQLuse_database statement12;     //使用数据库SQL语句
};

//主要函数：

//获取用户输入，并对输入进行解析，解析过程中对语句进行语法判断，返回内部语句格式
statement Read_Interpreter(char input[]);

#endif // Interpreter_H
