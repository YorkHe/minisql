/**
* @file  buffermanager.h
* @brief ����������ͷ�ļ�
* @author tgmerge
* ����������������黺�������Ƿ���Ҫ��Ŀ飬�����򷵻أ�����Ӵ��̶�ȡ��
* ΪIndex��Record�ṩ����Ŀ�
*
* ÿ��BufferManager����ֻ��һ�����ݿ⣨�ļ������ļ�����"���ݿ���.db"��
* ���췽����Ҫ�������ݿ�����
* ʹ��BufferManagerʱ����Ҫ����д�鵽�ļ���ֻ��Ҫ�ڿ鷢���ı�ʱ���ÿ��dirty()�����������ó����
* BufferManager������鱻�滻�����棬����������֮ǰ������д���ļ���
*/

/**
��Ŀ�����dbName.blk
��¼ÿ��table��һ��������һ�����ƫ����
�Ƕ������ļ�
tableName[MAX_TABLE_NAME]  Firstoffset[4]  Lastoffset[4]
tableName[MAX_TABLE_NAME]  Firstoffset[4]  Lastoffset[4]
...
*/


#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "block.h"

#include <string>
#include <iostream>
#include <list>
#include <hash_map>
#include <fstream>

using namespace std;

// ��������������
#define MAX_BLOCK_ACTIVE 40
#define MAX_TABLE_NAME   32

class BufferManager {

private:
	string dbName;
	string dbFileName;
	string infoFileName;
	fstream dbFile;
	fstream infoFile;
private:
	// �����һ���ƫ��string:tableName, int:offset
	hash_map<string, int> firstBlock;
	// �������һ���ƫ��string:tableName, int:offset
	hash_map<string, int> lastBlock;

public:
	// ok �ڴ��еĻ�����
	list<Block> buffer;

public:							// ���죬����
								// ok �����ݿ�����ʼ��buffermanager
	BufferManager(string name);
	// ok д��������飬����buffermanager
	virtual ~BufferManager();

public:						// ���÷�������д
							// ok ��ĳ�ļ���һ��block,���ܻ���
	Block readBlock(int offset);
	// ok ��blockд���ļ������Ǹɾ��ľͲ�д�ˣ�
	void writeBlock(Block &block);
	// ok д�����л������е���block���ļ�
	void writeAllBlocks();
	// ok �����������Ϣ
	void debug(bool isContent);

private:						// ���÷��������ҿ���أ�LRU
								// ok ��offset�ڻ�����ļ��в��ҿ飬�����뻺��
	Block& findBlock(int offset);

private:						// ���ã����±�����.blk
								// ok ��ȡdbFile�ı�������ļ�, ��ʧ�����½�һ��
	void readDbInfo();
	// ok д�����ļ�
	void writeDbInfo();

public:							// RecordManagerʹ��
								// ok ����tableName�����п��
	vector<int> getTableBlocks(string tableName);
	// ok ��tableName���һ���¿�,������firstLast��Ҳ���������
	Block& newBlock(string tableName);
	// ok
	/*+*/Block& getBlocks(int offset);
	// ok
	/*+*/void storeBlocks(int offset, Block& block);



public:						// IndexManagerʹ��
							// TODO ����indexName�����п�ƫ����
	vector<int> getIndexBlocks(string indexName);
	// ��tableName���һ���¿�
	Block& newIndexBlock(string indexName);
	// TODO
	/*+*/Block& getIndexOneBlock(string indexName, int offset);
	// 
	/*+*/void writeIndexData(string indexName, char* content, int length);

	/*+*/void writeIndexData(string indexName, int offset, char* content, int length);
};

#endif