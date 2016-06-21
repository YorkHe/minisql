/**
 * @file   block.h
 * @brief  �������ݿ�
 * @author tgmerge
 * 
 * ���յ�BufferManager���صĿ��
 * ʹ��getContent�����õ��ڲ����ݣ��ַ������飩ָ�룻
 * ʹ��getSize�����õ��ڲ����ݵ���Ч����
 */

/*
�ļ��еĿ�ṹ
[   HEAD   ][          CONTENT          ]
HEAD_LEN   64   HEAD�ĳ���
BLOCK_LEN  4096  CONTENT�ĳ���

HEAD�ṹ
TABLE_NAME      char*  32    �ÿ�ı���
OFFSET			int    4     ƫ����
NEXT_OFFSET     int    4     ��һ���ƫ����
CONTENT_SIZE    int    4     ������Ч���ݵĳ���
IS_ALIVE        bool   1     �ÿ��Ƿ��ѱ�ɾ��
IS_INDEX        bool   1     �Ƿ���index

CONTENT�ṹ
CONTENT         char*  4096  ����
*/


#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

//�����������ߴ�
#define BLOCK_LEN 4096
//��ͷ�ߴ�
#define HEAD_LEN  64

class Block {

public:						// ��ͷ����
	char tableName[32];			// ����
	int nextOffset;             // ��һ�����ļ��е�ƫ����
	int contentSize;			// ������Ч���ݳ���
	bool isAlive;               // �ÿ��Ƿ��ѱ�ɾ��
	bool isIndex;
public:						// ������
	char content[BLOCK_LEN];	// ����
public:						// ����
	int offset;					// ��block���ļ��е�ƫ����
	bool isDirty;				// �Ƿ���Ҫд���ļ�
	//bool isActive;				// for lru
	//int value;					// for lru

public:							// ���죬����
	// ��ʼ��һ���¿飬��������
	Block();
	// ָ���Ƿ���������ʼ��һ���¿�
	Block(bool index);
	// ���dirty����д���ļ�������

public:							// ���������з�����д����
	// �������������ģ��Ա�д���ļ�
	void dirty();

	// ��ȡ������ڲ�������
	char* getContent();

	// ��ȡ������ڲ������ݳ���
	int getSize();

	// ������
	void debug(bool withContent = false);
};

#endif