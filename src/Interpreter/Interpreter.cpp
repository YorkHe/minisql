#include<iostream>
#include<string>
#include"Interpreter.h"
#include<stdlib.h>
using namespace std;

//获取用户输入，并对输入进行解析，解析过程中对语句进行语法判断，返回内部语句格式
statement  Read_Interpreter(char input[])
{
    statement SQL;
    char *Split_Input;
    const char *d = " ,'\n'";
    Split_Input=strtok(input,d);
    string Input_splitp[100];
    int i=0;

    while(Split_Input)
    {
        Input_splitp[i]=Split_Input;
        Split_Input=strtok(NULL,d);
        i++;
    }

    if(!strcmp(Input_splitp[0].c_str(),"select"))
    {
        if(!strcmp(Input_splitp[1].c_str(),"*"))
        {
            if(!strcmp(Input_splitp[2].c_str(),"from"))
            {
                if(!strcmp(Input_splitp[4].c_str(),"where"))
                {
                    SQL.num=1;
                    SQL.statement1.table_name=Input_splitp[3];
                    SQL.statement1.condition_num=0;
                    for(int j=5;j<i;j=j+4)
                    {
                        SQL.statement1.select_condition[SQL.statement1.condition_num].column=Input_splitp[j];
                        SQL.statement1.select_condition[SQL.statement1.condition_num].op=Input_splitp[j+1];
                        if(strstr(Input_splitp[j+2].c_str(),"'"))
                        {
                            SQL.statement1.select_condition[SQL.statement1.condition_num].col_type=2;
                            Input_splitp[j+2]=Input_splitp[j+2].erase(0,1);
                            SQL.statement1.select_condition[SQL.statement1.condition_num].value2=Input_splitp[j+2].erase(Input_splitp[j+2].length()-1,1);
                            SQL.statement1.condition_num++;
                        }
                        else if(strstr(Input_splitp[j+2].c_str(),"."))
                        {
                            SQL.statement1.select_condition[SQL.statement1.condition_num].col_type=3;
                            SQL.statement1.select_condition[SQL.statement1.condition_num].value3=atof(Input_splitp[j+2].c_str());
                            SQL.statement1.condition_num++;
                        }
                        else
                        {
                            SQL.statement1.select_condition[SQL.statement1.condition_num].col_type=1;
                            SQL.statement1.select_condition[SQL.statement1.condition_num].value3=atoi(Input_splitp[j+2].c_str());
                            SQL.statement1.condition_num++;
                        }
                    }
                    return SQL;
                }
                else
                {
                    if(i==4)
                    {
                        SQL.num=1;
                        SQL.statement1.table_name=Input_splitp[3];
                        SQL.statement1.condition_num=0;
                        return SQL;
                    }
                    else
                    {
                        SQL.num=0;
                        return SQL;
                    }
                }
            }
            else
            {
                SQL.num=0;
                return SQL;
            }
        }
        else
        {
            SQL.num=0;
            return SQL;
        }
    }
    else if(!strcmp(Input_splitp[0].c_str(),"delete"))
    {
        if(!strcmp(Input_splitp[1].c_str(),"from"))
        {
            if(!strcmp(Input_splitp[3].c_str(),"where"))
            {
                SQL.num=2;
                SQL.statement2.table_name=Input_splitp[2];
                SQL.statement2.condition_num=0;
                for(int j=4;j<i;j=j+4)
                {
                    SQL.statement2.delete_condition[SQL.statement2.condition_num].column=Input_splitp[j];
                    SQL.statement2.delete_condition[SQL.statement2.condition_num].op=Input_splitp[j+1];
                    if(strstr(Input_splitp[j+2].c_str(),"'"))
                    {
                        SQL.statement2.delete_condition[SQL.statement2.condition_num].col_type=2;
                        Input_splitp[j+2]=Input_splitp[j+2].erase(0,1);
                        SQL.statement2.delete_condition[SQL.statement2.condition_num].value2=Input_splitp[j+2].erase(Input_splitp[j+2].length()-1,1);
                        SQL.statement2.condition_num++;
                    }
                    else if(strstr(Input_splitp[j+2].c_str(),"."))
                    {
                        SQL.statement2.delete_condition[SQL.statement2.condition_num].col_type=3;
                        SQL.statement2.delete_condition[SQL.statement2.condition_num].value3=atof(Input_splitp[j+2].c_str());
                        SQL.statement2.condition_num++;
                    }
                    else
                    {
                        SQL.statement2.delete_condition[SQL.statement2.condition_num].col_type=1;
                        SQL.statement2.delete_condition[SQL.statement2.condition_num].value3=atoi(Input_splitp[j+2].c_str());
                        SQL.statement2.condition_num++;
                    }
                }
                return SQL;
            }
            else
            {
                if(i==3)
                {
                    SQL.num=2;
                    SQL.statement2.condition_num=0;
                    SQL.statement2.table_name=Input_splitp[2];
                    return SQL;
                }
                else
                {
                    SQL.num=0;
                    return SQL;
                }
            }
        }
        else
        {
            SQL.num=0;
            return SQL;
        }
    }
    else if(!strcmp(Input_splitp[0].c_str(),"insert"))
    {
        if(!strcmp(Input_splitp[1].c_str(),"into")&&!strcmp(Input_splitp[3].c_str(),"values"))
        {
            SQL.num=3;
            SQL.statement3.table_name=Input_splitp[2];
            SQL.statement3.Values_num=0;
            Input_splitp[4]=Input_splitp[4].erase(0,1);
            Input_splitp[i-1]=Input_splitp[i-1].erase(Input_splitp[i-1].length()-1,1);
            for(int j=4;j<i;j++)
            {
                if(strstr(Input_splitp[j].c_str(),"'"))
                {
                    SQL.statement3.Insert_values[SQL.statement3.Values_num].col_type=2;
                    Input_splitp[j]=Input_splitp[j].erase(0,1);
                    SQL.statement3.Insert_values[SQL.statement3.Values_num].value2=Input_splitp[j].erase(Input_splitp[j].length()-1,1);
                    SQL.statement3.Values_num++;
                }
                else if(strstr(Input_splitp[j].c_str(),"."))
                {
                    SQL.statement3.Insert_values[SQL.statement3.Values_num].col_type=3;
                    SQL.statement3.Insert_values[SQL.statement3.Values_num].value3=atof(Input_splitp[j].c_str());
                    SQL.statement3.Values_num++;
                }
                else
                {
                    SQL.statement3.Insert_values[SQL.statement3.Values_num].col_type=1;
                    SQL.statement3.Insert_values[SQL.statement3.Values_num].value3=atoi(Input_splitp[j].c_str());
                    SQL.statement3.Values_num++;
                }
            }
        }
        else
        {
            SQL.num=0;
            return SQL;
        }
    }
    else if(!strcmp(Input_splitp[0].c_str(),"execfile"))
    {
        if(i==2)
        {
            SQL.num=4;
            SQL.statement4.filename=Input_splitp[1];
            return SQL;
        }
        else
        {
            SQL.num=0;
            return SQL;
        }
    }
    else if(!strcmp(Input_splitp[0].c_str(),"create"))
    {
        if(!strcmp(Input_splitp[1].c_str(),"table"))
        {
            if(!strcmp(Input_splitp[3].c_str(),"(")&&strcmp(Input_splitp[i-1].c_str(),")"))
            {
                SQL.num=5;
                SQL.statement5.table_name=Input_splitp[2];
                SQL.statement5.Attribute_num=0;
                const char* dd = " ,'\n'()";
                char* Table_Input_Split;
                Table_Input_Split=strtok(input,dd);
                string Input_Table_Split[100];
                int j=0;
                while(Table_Input_Split)
                {
                    Input_Table_Split[j]=Table_Input_Split;
                    Table_Input_Split=strtok(NULL,dd);
                    j++;
                }
                string compare_char="char";
                string compare_unique="unique";
                string compare_int="int";
                string compare_float="float";
                string compare_primary="primary";
                for(int t = 3; t < j; t++ )
                {
                    if( Input_Table_Split[t] == compare_char )
                    {
                        SQL.statement5.attribute[SQL.statement5.Attribute_num].column_type=2;
                        SQL.statement5.attribute[SQL.statement5.Attribute_num].char_num=atoi(Input_Table_Split[t+1].c_str());
                        SQL.statement5.attribute[SQL.statement5.Attribute_num].column_name=Input_Table_Split[t-1];
                        if(Input_Table_Split[t+2]== compare_unique)
                            SQL.statement5.attribute[SQL.statement5.Attribute_num].Is_unique=1;
                        else
                            SQL.statement5.attribute[SQL.statement5.Attribute_num].Is_unique=-1;

                        SQL.statement5.Attribute_num++;
                    }
                    if( Input_Table_Split[t] == compare_int )
                    {
                        SQL.statement5.attribute[SQL.statement5.Attribute_num].column_type=1;
                        SQL.statement5.attribute[SQL.statement5.Attribute_num].column_name=Input_Table_Split[t-1];
                        if(Input_Table_Split[t+1]== compare_unique)
                            SQL.statement5.attribute[SQL.statement5.Attribute_num].Is_unique=1;
                        else
                            SQL.statement5.attribute[SQL.statement5.Attribute_num].Is_unique=-1;

                        SQL.statement5.Attribute_num++;
                    }
                    if( Input_Table_Split[t] == compare_float )
                    {
                        SQL.statement5.attribute[SQL.statement5.Attribute_num].column_type=3;
                        SQL.statement5.attribute[SQL.statement5.Attribute_num].column_name=Input_Table_Split[t-1];
                        if(Input_Table_Split[t+1]== compare_unique)
                            SQL.statement5.attribute[SQL.statement5.Attribute_num].Is_unique=1;
                        else
                            SQL.statement5.attribute[SQL.statement5.Attribute_num].Is_unique=-1;

                        SQL.statement5.Attribute_num++;
                    }
                    if( Input_Table_Split[t] == compare_primary )
                    {
                        SQL.statement5.primary_key=Input_Table_Split[t+2];
                    }
                }
                return SQL;
            }
            else
            {
                SQL.num=0;
                return SQL;
            }
        }
        else if(!strcmp(Input_splitp[1].c_str(),"index"))
        {
            if(i==8)
            {
                if(!strcmp(Input_splitp[3].c_str(),"on")&&!strcmp(Input_splitp[5].c_str(),"(")&&!strcmp(Input_splitp[7].c_str(),")"))
                {
                    SQL.num=6;
                    SQL.statement6.index_name=Input_splitp[2];
                    SQL.statement6.table_name=Input_splitp[4];
                    SQL.statement6.column_name=Input_splitp[6];
                    return SQL;
                }
                else
                {
                    SQL.num=0;
                    return SQL;
                }
            }
            else
            {
                SQL.num=0;
                return SQL;
            }
        }
        else if(!strcmp(Input_splitp[1].c_str(),"database"))
        {
            if(i==3)
            {
                SQL.num=10;
                SQL.statement10.database_name=Input_splitp[2];
                return SQL;
            }
            else
            {
                SQL.num=0;
                return SQL;
            }
        }
        else
        {
            SQL.num=0;
            return SQL;
        }
    }
    else if(!strcmp(Input_splitp[0].c_str(),"drop"))
    {
        if(!strcmp(Input_splitp[1].c_str(),"table"))
        {
            if(i==3)
            {
                SQL.num=7;
                SQL.statement7.table_name=Input_splitp[2];
                return SQL;
            }
            else
            {
                SQL.num=0;
                return SQL;
            }
        }
        else if(!strcmp(Input_splitp[1].c_str(),"index"))
        {
            if(i==3)
            {
                SQL.num=8;
                SQL.statement8.index_name=Input_splitp[2];
                return SQL;
            }
            else
            {
                SQL.num=0;
                return SQL;
            }
        }
        else if(!strcmp(Input_splitp[1].c_str(),"database"))
        {
            if(i==3)
            {
                SQL.num=11;
                SQL.statement11.database_name=Input_splitp[2];
                return SQL;
            }
            else
            {
                SQL.num=0;
                return SQL;
            }
        }
        else
        {
            SQL.num=0;
            return SQL;
        }
    }
    else if(!strcmp(Input_splitp[0].c_str(),"quit"))
    {
        SQL.num=9;
        return SQL;
    }
    else if(!strcmp(Input_splitp[0].c_str(),"use"))
    {
        if(i==2)
        {
            SQL.num=12;
            SQL.statement12.database_name=Input_splitp[1];
            return SQL;
        }
        else
        {
            SQL.num=0;
            return SQL;
        }
    }
    SQL.num=0;
    return SQL;
}
