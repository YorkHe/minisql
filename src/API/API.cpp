#include<iostream>
#include"API.h"
#include<vector>
#include<cstring>
#include<fstream>

using namespace std;

void Excute_SQL(statement SQL,string& name_of_database)      //结合Record Manager，Index Manager，Catalog Manage模块提供的接口执行语句
{
    CatalogManager Catalog;
    CError Is_Error;
    Condition cond;
    if(SQL.num==10)//创建数据库
    {
        try{
        Is_Error=Catalog.createDatabase(SQL.statement10.database_name);
        if(Is_Error.err_code)
        {
            cout<<Is_Error.description;
        }
        else
        {
            cout<<"create database "<<SQL.statement10.database_name<<" successfully!"<<endl;
        }
        }catch(CError c_err){
            cout<<c_err.description;
        }
    }
    else if(SQL.num==11)//删除数据库
    {
        try{
        Is_Error=Catalog.dropDatabase(SQL.statement11.database_name);
        if(Is_Error.err_code)
        {
            cout<<Is_Error.description;
        }
        else
        {
            cout<<"drop database "<<SQL.statement11.database_name<<" successfully!"<<endl;
        }
        }
        catch(CError c_err)
        {
            cout<<c_err.description;
        }
    }
    else if(SQL.num==12)//使用数据库
    {
        try{
        Is_Error=Catalog.useDatabase(SQL.statement12.database_name);
        if(Is_Error.err_code)
        {
            cout<<Is_Error.description;
        }
        else
        {
            cout<<"Database changed"<<endl;
            name_of_database=SQL.statement12.database_name;
        }
        }
        catch(CError c_err)
        {
            cout<<c_err.description;
        }
    }
    else if(SQL.num==7)//删除表
    {
        try{
        Is_Error=Catalog.dropTable(name_of_database,SQL.statement7.table_name);
        if(Is_Error.err_code)
        {
            cout<<Is_Error.description;
        }
        else
        {
            cout<<"Drop table "<<SQL.statement7.table_name<<" successfully!"<<endl;
        }
        }
        catch(CError c_err)
        {
            cout<<c_err.description;
        }
    }
    else if(SQL.num==8)//删除索引
    {
        try{
        Is_Error=Catalog.dropIndex(name_of_database,SQL.statement8.index_name);
        if(Is_Error.err_code)
        {
            cout<<Is_Error.description;
        }
        else
        {
            cout<<"Drop index "<<SQL.statement8.index_name<<" successfully!"<<endl;
        }
        }
        catch(CError c_err)
        {
            cout<<c_err.description;
        }
    }
    else if(SQL.num==2)//删除记录
    {
        if(SQL.statement2.condition_num==0)//删除表的所有记录
        {
            try{
            Is_Error=Catalog.emptyTable(name_of_database,SQL.statement2.table_name);
            if(Is_Error.err_code)
            {
                cout<<Is_Error.description;
            }
            else
            {
                cout<<"Empty table "<<SQL.statement2.table_name<<" successfully!"<<endl;
            }
            }
            catch(CError c_err)
            {
                cout<<c_err.description;
            }
        }
        else//根据where子句删除特定记录
        {
            RecordManager record(name_of_database,SQL.statement2.table_name);
            cond.with_where=true;
            for(int i=0;i<SQL.statement2.condition_num;i++)
            {
                Formula tmp_where;
                tmp_where.attr_1=SQL.statement2.delete_condition[i].column;
                tmp_where.attr_2=SQL.statement2.delete_condition[i].value;
                if(SQL.statement2.delete_condition[i].op=="=")
                {
                    tmp_where.relationship=EQUAL;
                }
                else if(SQL.statement2.delete_condition[i].op==">")
                {
                    tmp_where.relationship=GREATER;
                }
                else if(SQL.statement2.delete_condition[i].op=="<")
                {
                    tmp_where.relationship=LESS;
                }
                else if(SQL.statement2.delete_condition[i].op=="<=")
                {
                    tmp_where.relationship=LESS_AND_EQUAL;
                }
                else if(SQL.statement2.delete_condition[i].op==">=")
                {
                    tmp_where.relationship=GREATE_AND_EQUAL;
                }
                else if(SQL.statement2.delete_condition[i].op=="<>")
                {
                    tmp_where.relationship=NOT_EQUAL;
                }
                cond.where_clause.push_back(tmp_where);
            }
            try{
            Is_Error=record.deleteRecord(cond);
            if(Is_Error.err_code)
            {
                cout<<Is_Error.description;
            }
            else
            {
                cout<<"Delete record successfully!"<<endl;
            }
            }
            catch(CError c_err)
            {
                cout<<c_err.description;
            }
        }
    }
    else if(SQL.num==3)//插入语句
    {
        RecordManager record(name_of_database,SQL.statement3.table_name);
        vector <string> tmp;
        tmp=Catalog.getAllAttributes(name_of_database,SQL.statement3.table_name);
        Tuple tmp_tuple;
        for(unsigned int i;i<tmp.size();i++)
        {
            tmp_tuple.element[i].attr_name=tmp[i];
            tmp_tuple.element[i].value=SQL.statement3.Insert_values[i].value;
        }
        try{
        Is_Error=record.insertRecord(tmp_tuple);
        if(Is_Error.err_code)
        {
            cout<<Is_Error.description;
        }
        else
        {
            cout<<"Insert record successfully!"<<endl;
        }
        }
        catch(CError c_err)
        {
            cout<<c_err.description;
        }
    }
    else if(SQL.num==6)//建立索引
    {

        try{
        Is_Error=Catalog.createIndex(name_of_database,SQL.statement6.table_name,SQL.statement6.column_name,SQL.statement6.index_name);
        if(Is_Error.err_code)
        {
            cout<<Is_Error.description;
        }
        else
        {
            cout<<"Create index "<<SQL.statement6.index_name<<" successfully!"<<endl;
        }
        }
        catch(CError c_err)
        {
            cout<<c_err.description;
        }
    }
    else if(SQL.num==1)//选择语句
    {
        RecordManager record(name_of_database,SQL.statement2.table_name);
        if(SQL.statement1.condition_num==0)
        {
            cond.with_where=false;
            try
            {
                Is_Error=record.selectRecord(cond);
                if(Is_Error.err_code)
                {
                    cout<<Is_Error.description;
                }
            }
            catch(CError c_err)
            {
                cout<<c_err.description;
            }
        }
        else
        {
            cond.with_where=true;
            for(int i=0;i<SQL.statement1.condition_num;i++)
            {
                Formula temp_where;
                temp_where.attr_1=SQL.statement1.select_condition[i].column;
                temp_where.attr_2=SQL.statement1.select_condition[i].value;
                if(SQL.statement1.select_condition[i].op=="=")
                {
                    temp_where.relationship=EQUAL;
                }
                else if(SQL.statement1.select_condition[i].op==">")
                {
                    temp_where.relationship=GREATER;
                }
                else if(SQL.statement1.select_condition[i].op=="<")
                {
                    temp_where.relationship=LESS;
                }
                else if(SQL.statement1.select_condition[i].op=="<=")
                {
                    temp_where.relationship=LESS_AND_EQUAL;
                }
                else if(SQL.statement1.select_condition[i].op==">=")
                {
                    temp_where.relationship=GREATE_AND_EQUAL;
                }
                else if(SQL.statement1.select_condition[i].op=="<>")
                {
                    temp_where.relationship=NOT_EQUAL;
                }
                cond.where_clause.push_back(temp_where);
            }
            try
            {
                Is_Error=record.selectRecord(cond);
                if(Is_Error.err_code)
                {
                    cout<<Is_Error.description;
                }
            }
            catch(CError c_err)
            {
                cout<<c_err.description;
            }
        }
    }
    else if(SQL.num==4)//执行文件
    {
        ifstream fp(SQL.statement4.filename.c_str());
        if(!fp.is_open())
        {
            cout<<"Open file error!"<<endl;
            return;
        }
        char input[3000];
        char buffer[256];
        while(!fp.eof())
        {
            fp.getline(buffer,100);
            strcat(input,buffer);
        }
        char *Split_Input;
        const char *d = ";";
        Split_Input=strtok(input,d);
        string Input_splitp[100];
        int i=0;

        while(Split_Input)
        {
            Input_splitp[i]=Split_Input;
            Split_Input=strtok(NULL,d);
            i++;
        }

        for(int j=0;j<i;j++)
        {
            char Temp[300];
            strcpy(Temp,Input_splitp[j].c_str());
            statement new_SQL=Read_Interpreter(Temp);
            Excute_SQL(new_SQL,name_of_database);
        }
    }
    else if(SQL.num==5)//创建表(没写完)
    {
        vector<Attr> attr;
        int tmp=-1;
        for(int j=0;j<SQL.statement5.Attribute_num;j++)
        {
            if(SQL.statement5.primary_key==SQL.statement5.attribute[j].column_name)
            {
                tmp=j;
                break;
            }
        }
        for(int i=0;i<SQL.statement5.Attribute_num;i++)
        {
            Attr New_attr;
            New_attr.name=SQL.statement5.attribute[i].column_name;
            switch(SQL.statement5.attribute[i].column_type)
            {
            case 1:
                {
                    New_attr.type_id=0;
                    break;
                }
            case 2:
                {
                    New_attr.type_id=1;
                    break;
                }
            case 3:
                {
                    New_attr.type_id=2;
                    break;
                }
            default:
                {
                    break;
                }
            }
            New_attr.is_reference=false;
            if(SQL.statement5.attribute[i].Is_unique==1)
            {
                New_attr.is_unique=true;
            }
            else if(SQL.statement5.attribute[i].Is_unique==-1)
            {
                New_attr.is_unique=false;
            }
            if(i==tmp)
            {
                New_attr.is_prime=true;
            }
            else
            {
                New_attr.is_prime=false;
            }
            attr.push_back(New_attr);
        }
        try
        {
            Is_Error=Catalog.createTable(name_of_database,SQL.statement5.table_name,attr);
            if(Is_Error.err_code)
            {
                cout<<Is_Error.description;
            }
            else
            {
                cout<<"Create table "<<SQL.statement5.table_name<<" successfully!"<<endl;
            }
        }
        catch(CError c_err)
        {
            cout<<c_err.description;
        }
    }
}