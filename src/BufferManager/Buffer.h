//
//  Buffer.hpp
//  BufferManage
//
//  Created by 章世超 on 6/12/16.
//  Copyright © 2016 章世超. All rights reserved.
//

#ifndef Buffer_h
#define Buffer_h

//#include "idef.h"
#include <string>
using namespace std;
class buffer{
public:
    bool is_written;
    bool is_data_exist;
    bool is_locked;
    string file_name;
    int block_offset;
    int LRU;
    char values[4096 + 1];
    buffer(){
        initialize();
    }
    void initialize(){
        is_written  = 0;
        is_data_exist = 0;
        is_locked=0;
        file_name = "NULL";
        block_offset = 0;
        LRU= 0;
        for(int i = 0; i<4096; i++) values[i] = '#';
        values[4096] = '\0';
    }
    string getvalues(int begin, int end){
        string temp = "";
        if(begin >=0 && begin <= end && end <= 4096)
            for(int i = begin; i< end; i++)
                temp = temp + values[i];
        return temp;
    }
    char getvalues(int index){
        if(index >=0 && index <=4096)
            return values[index];
        return '\0';
    }
};
#endif /* Buffer_hpp */
