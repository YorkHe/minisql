#ifndef API_H
#define API_H
#include "Interpreter/Interpreter.h"
#include "CatalogManger/CatalogManger.h"
#include "IndexManager/IndexManager.h"
#include "RecordManager/RecordManager.h"
#include "utility.h"

void Excute_SQL(statement SQL,string& name);      //���Record Manager��Index Manager��Catalog Manageģ���ṩ�Ľӿ�ִ�����

#endif // API_H