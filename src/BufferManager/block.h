/*
文件中的块结构
[   HEAD   ][          CONTENT          ]
HEAD_LEN   64   
BLOCK_LEN  4096  

HEAD结构
TABLE_NAME      char*  32    该块的表名
OFFSET			int    4     偏移量
NEXT_OFFSET     int    4     下一块的偏移量
CONTENT_SIZE    int    4     块内有效数据的长度
IS_ALIVE        bool   1     该块是否已被删除
IS_INDEX        bool   1     是否是index

CONTENT结构
CONTENT         char*  4096  数据
*/


#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;


#define BLOCK_LEN 4096

#define HEAD_LEN  64

class Block {

public:						
	char tableName[32];		
	int nextOffset;     
	int contentSize;
	bool isAlive;  
	bool isIndex;

	char content[BLOCK_LEN];

	int offset;		
	bool isDirty;
	bool isActive;		
	int value;				

	Block();

	Block(bool index);

	void dirty();

	char* getContent();

	int getSize();

	void debug(bool withContent = false);
};

#endif