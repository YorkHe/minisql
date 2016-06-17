//
//  BufferManage.cpp
//  BufferManage
//
//  Created by 章世超 on 6/12/16.
//  Copyright © 2016 章世超. All rights reserved.
//

#include "Buffer.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;
//path of file, database , but it have to change arguments

class BufferManager
{
public:
    
    BufferManager(){
        for(int i = 0; i< 1000; i++) buffer_block[i].initialize();
    }
    ~BufferManager(){
        for(int i = 0; i< 1000; i++)
            flashBack(i);
    }
private:
public:
    buffer buffer_block[1000]; // OK
    
    int getBufferNum(string file_name, int block_offset){//OK
        int buffer_num = checkInBuffer(file_name, block_offset); //check whether some block_offset of some file is in the buffer
        if(buffer_num == -1){  //not in the buffer
            buffer_num = getFreeBuffer(file_name);//get a free buffer
            readBlock(file_name, block_offset, buffer_num);//read the file into the free buffer we get
        }
        return buffer_num;//return the index of the buffer
    }
    
    int checkInBuffer(string file_name, int block_offset){
        ////check whether some block_offset of some file is in the buffer
        for(int buffer_num = 0; buffer_num < 1000; buffer_num++)// seek through the buffer
            if(buffer_block[buffer_num].file_name == file_name && buffer_block[buffer_num].block_offset == block_offset)	return buffer_num; // find the file block
        return -1;	//the file block is not in the buffer
    }
    
    
    int getFreeBuffer(){
        int buffer_num = 0;
        int highestLRU = buffer_block[0].LRU;//initialize the highestLRU
        for(int i = 0; i < 1000; i++)
        {
            if(!buffer_block[i].is_data_exist){ //find a buffer in which data not exist
                buffer_block[i].initialize();
                buffer_block[i].is_data_exist = 1;
                return i;
            }
            else if(highestLRU < buffer_block[i].LRU&&!buffer_block[i].is_locked)
            {
                highestLRU = buffer_block[i].LRU;
                buffer_num = i;//find the buffer which has the highest LRU
            }
        }
        flashBack(buffer_num);//flash the buffer which has the highest LRU
        buffer_block[buffer_num].is_data_exist = 1; //mark the buffer to be stored with data
        return buffer_num;
    }
    
    int getFreeBuffer(string file_name){	//buffer with the destine file_name is not suppose to be flashback
        int buffer_num = 0;
        int highestLRU = buffer_block[0].LRU;
        for(int i = 0; i < 1000; i++)
        {
            if(!buffer_block[i].is_data_exist){
                buffer_block[i].is_data_exist = 1; //ok
                return i;
            }
            else if(highestLRU < buffer_block[i].LRU && buffer_block[i].file_name != file_name&&!buffer_block[i].is_locked)
            {
                highestLRU = buffer_block[i].LRU;
                buffer_num = i;
            }
        }
        flashBack(buffer_num);
        buffer_block[buffer_num].is_data_exist = 1;
        return buffer_num;
    } //this function is reloaded with the function getFreeBuffer();
    
    
    
    void flashBack(int buffer_num){  //flashBack the buffer which has the highest LRU
        if(!buffer_block[buffer_num].is_written) return;
        //if the buffer is written, that is, it is the same as the file block in the disk , we don't need to write it to the disk
        string file_name = buffer_block[buffer_num].file_name;
        fstream fout(file_name.c_str(), ios::in|ios::out);  //find the file
        fout.seekp(4096*buffer_block[buffer_num].block_offset, fout.beg);//seek through the file
        fout.write(buffer_block[buffer_num].values, 4096);
        buffer_block[buffer_num].initialize();
        fout.close();
    }
    
    
    
    void readBlock(string file_name, int block_offset, int buffer_num){
        buffer_block[buffer_num].is_data_exist = 1;
        buffer_block[buffer_num].is_written = 0;
        buffer_block[buffer_num].file_name = file_name;
        buffer_block[buffer_num].block_offset = block_offset;
        fstream  fin(file_name.c_str(), ios::in);
        fin.seekp(4096*block_offset, fin.beg);
        fin.read(buffer_block[buffer_num].values, 4096);
        fin.close();
    }

    
    
    void writeAll(string file_name,int file_buffer_num){
        fstream  fin(file_name.c_str(), ios::in);
        for(int block_offset = 0; block_offset < file_buffer_num; block_offset++){
            if(checkInBuffer(file_name, block_offset) == -1){//if the file buffer is not in the buffer manage
                int buffer_num = getFreeBuffer(file_name);
                readBlock(file_name, block_offset, buffer_num);
            }
        }
        fin.close();
    }
    
    void updateLRU(int buffer_num){
        for(int i = 1; i < 1000; i++)
        {
            if(i == buffer_num ){
                buffer_block[buffer_num].LRU = 0;
                buffer_block[i].is_data_exist = 1;
            }
            else buffer_block[i].LRU++;	//The greater is LRU, the less useful the block is
        }
    }

   
    void setNotDataExist(string file_name){//when a file is deleted, a table or an index, all the value in buffer should be set invalid
        for(int i = 0; i <1000; i++)
        {
            if(buffer_block[i].file_name == file_name){
                buffer_block[i].is_data_exist = 0;
                buffer_block[i].is_written = 0;
            }
        }
    }
    
    void setLocked(int buffer_num){
        buffer_block[buffer_num].is_locked=1;
    }
    
    void setUnLocked(int buffer_num){
        buffer_block[buffer_num].is_locked=0;
    }
    
    void setWritten(int buffer_num){
        buffer_block[buffer_num].is_written=1;
        updateLRU(buffer_num);
    }
public:
    //for test
    
    void ShowBuffer(int buffer_num){
        cout << "BlockNum: " << buffer_num << endl;
        cout << "is_written: " << buffer_block[buffer_num].is_written << endl;
        cout << "is_data_exist: " << buffer_block[buffer_num].is_data_exist << endl;
        cout << "is_locked: " << buffer_block[buffer_num].is_locked << endl;
        cout << "file_name: " << buffer_block[buffer_num].file_name << endl;
        cout << "block_offset: " << buffer_block[buffer_num].block_offset << endl;
        cout<<"LRU:"<<buffer_block[buffer_num].LRU<<endl;
        cout << buffer_block[buffer_num].values <<endl;
    }
   
};