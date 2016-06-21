#include<iostream>
#include"API/API.h"

using namespace std;

int main()
{
    char input[500];
    memset(input,'\0',sizeof(input));
    cout<<"Welcome to use MiniSQL!"<<endl<<"-------------------------"<<endl<<endl;
    string name_of_datadase="master";
    statement SQL;
    SQL.num=-1;

    while(1)
    {
        cout<<"MinSQL->";
        int t=0;
        fflush(stdin);
        while(cin.peek()!=';')
        {
            if((input[t++]=cin.get()) == '\n')
                cout<<"MinSQL->";
        }
        fflush(stdin);

        SQL=Read_Interpreter(input);   //将用户输入解析成内置语句

        if(SQL.num==0)                   //如果输入错误则返回错误原因，并重置，等待下一个命令的输入
        {
            cout<<"error: SQL语句输入语法错误！"<<endl;
            memset(input,'\0',sizeof(input));
            SQL.num=-1;
        }
        else if(SQL.num==9)
        {
            break;
        }
        else
        {
            Excute_SQL(SQL,name_of_datadase);
            memset(input,'\0',sizeof(input));
            SQL.num=-1;
        }
    }

    return 0;
}