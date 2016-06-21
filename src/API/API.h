#ifndef API_H
#define API_H
#include "Interpreter.h"
#include "CatalogManager.h"
#include "IndexManager.h"
#include "RecordManager.h"
#include<iostream>

void Excute_SQL(statement SQL,string& name);      //结合Record Manager，Index Manager，Catalog Manage模块提供的接口执行语句

#endif // API_H
