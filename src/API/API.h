#ifndef API_H
#define API_H
#include "Interpreter/Interpreter.h"
#include "CatalogManger/CatalogManger.h"
#include "IndexManager/IndexManager.h"
#include "RecordManager/RecordManager.h"
#include "utility.h"

void Excute_SQL(statement SQL,string& name);      //结合Record Manager，Index Manager，Catalog Manage模块提供的接口执行语句

#endif // API_H