#include "buffermanager.h"

/**
 * @brief  �����ݿ����½�buffer��
 */
BufferManager::BufferManager(string name)
{
	dbName = name;
	dbFileName = DB_FILE(name) + ".db";
	infoFileName = DB_FILE(name)+ ".info";
	dbFile.open(dbFileName, std::fstream::in | std::fstream::out);
	if (!dbFile.is_open())
	{
		dbFile.open(dbFileName, std::ios::in | std::ios::out | std::ios::trunc);
	}
	infoFile.open(infoFileName, std::fstream::in | std::fstream::out);
	if (!infoFile.is_open())
	{
		infoFile.open(infoFileName, std::ios::in | std::ios::out | std::ios::trunc);
	}
	readDbInfo();
}

/**
 * @brief  �ر�buffer֮ǰ����bufferд���������
 */
BufferManager::~BufferManager()
{
	writeAllBlocks();
	writeDbInfo();
}

/**
 * @brief  �����ݿ��ļ���ȡһ��block��ָ��ƫ����
 */
Block BufferManager::readBlock(int offset)
{
	Block block;

	// ����ͷ
	dbFile.seekg(offset);
	dbFile.read(block.tableName, MAX_TABLE_NAME);
	dbFile.read(reinterpret_cast<char *>(&(block.offset)), sizeof(int));
	dbFile.read(reinterpret_cast<char *>(&(block.nextOffset)), sizeof(int));
	dbFile.read(reinterpret_cast<char *>(&(block.contentSize)), sizeof(int));
	dbFile.read(reinterpret_cast<char *>(&(block.isAlive)), sizeof(bool));
	dbFile.read(reinterpret_cast<char *>(&(block.isIndex)), sizeof(bool));

	// ����
	dbFile.seekg(offset + HEAD_LEN);
	dbFile.read(block.content, BLOCK_LEN);

	//
	block.isDirty = false;

	return block;
}


void BufferManager::writeBlock(Block& block)
{
	// �������ϴ��������д
	if (!block.isDirty)
	{
		return;
	}
	else
	{
		block.isDirty = false;
	}

	// д��ͷ
	dbFile.seekp(block.offset);
	dbFile.write(block.tableName, MAX_TABLE_NAME);
	dbFile.write(reinterpret_cast<char *>(&(block.offset)), sizeof(int));
	dbFile.write(reinterpret_cast<char *>(&(block.nextOffset)), sizeof(int));
	dbFile.write(reinterpret_cast<char *>(&(block.contentSize)), sizeof(int));
	dbFile.write(reinterpret_cast<char *>(&(block.isAlive)), sizeof(bool));
	dbFile.write(reinterpret_cast<char *>(&(block.isIndex)), sizeof(bool));

	// д��
	dbFile.seekp(block.offset + HEAD_LEN);
	dbFile.write(block.content, BLOCK_LEN);

	return;
}

void BufferManager::writeAllBlocks()
{
	for (list<Block>::iterator i = buffer.begin(); i != buffer.end(); ++i)
	{
		writeBlock(*i);
	}
	return;
}


Block& BufferManager::findBlock(int offset)
{
	// ���ڻ����в��ң��ҵ�������ڻ�����ǰͷ
	for (list<Block>::iterator i = buffer.begin(); i != buffer.end(); ++i)
	{
		if (i->offset == offset)
		{
			buffer.splice(buffer.begin(), buffer, i, std::next(i));
			return (buffer.front());
		}
	}
	// ��������д��ɾ������ĩβ�飬���ļ������뻺��
	if (buffer.size() >= MAX_BLOCK_ACTIVE)
	{
		writeBlock(*(buffer.end()));
		buffer.pop_back();
	}
	buffer.push_front(readBlock(offset));

	return (buffer.front());
}


void BufferManager::readDbInfo()
{
	char tableName[MAX_TABLE_NAME];
	string s;
	int offset = 0;

	infoFile.seekg(0, ios::beg);
	infoFile.read(tableName, MAX_TABLE_NAME);
	if (infoFile.eof())
		return;

	infoFile.seekg(0, ios::beg);
	while (!infoFile.eof())
	{
		infoFile.read(tableName, MAX_TABLE_NAME);
		s = tableName;
		infoFile.read(reinterpret_cast<char*>(offset), sizeof(int));
		firstBlock.insert(pair<string, int>(s, offset));
		infoFile.read(reinterpret_cast<char*>(offset), sizeof(int));
		lastBlock.insert(pair<string, int>(s, offset));
	}
}


void BufferManager::writeDbInfo()
{
	// ����ļ�
	infoFile.close();
	infoFile.open(infoFileName, std::fstream::out | std::fstream::in | std::fstream::trunc);
	for (
		hash_map<string, int>::iterator i = firstBlock.begin(), j = lastBlock.begin();
		i != firstBlock.end();
		++i , ++j
	)
	{
		//cout << (char *)((i->first).c_str()) << (i->first).size() << endl;
		//cout << (i->second) << endl;
		infoFile.write((i->first).c_str(), (i->first).size());
		infoFile.write(reinterpret_cast<char *>(&(i->second)), sizeof(int));
		infoFile.write(reinterpret_cast<char *>(&(j->second)), sizeof(int));
	}
}


vector<int> BufferManager::getTableBlocks(string tableName)
{
	//ת��tableNameΪchar[]
	hash_map<string, int>::iterator i;
	int offset;

	//��firstBlock�в���tableName�ĵ�һ���ַ
	for (i = firstBlock.begin(); i != firstBlock.end(); i ++)
	{
		if (tableName == i->first)
		{
			offset = i->second;
			break;
		}
	}

	if (i == firstBlock.end())
	{
		return vector<int>();
		// TODO δ�ҵ�����
	}

	//����vector�����Ҹ����в���index�Ŀ�
	vector<int> result;

	do
	{
		Block& block = findBlock(offset);
		if (block.isAlive)
		{
			result.push_back(offset);
		}
		offset = block.nextOffset;
	}
	while (offset != 0);

	return result;
}

Block& BufferManager::newBlock(string tableName)
{
	//ת��tableNameΪchar[]
	const char* tableChar = tableName.c_str();
	hash_map<string, int>::iterator i;

	//�½���Block��tableΪtableName, offsetΪdb�ļ�ĩβ����ֱ��д���ļ�ĩβ
	Block block;
	memcpy(block.tableName, tableChar, MAX_TABLE_NAME);
	dbFile.seekp(0, ios_base::end);
	block.offset = dbFile.tellp();
	writeBlock(block);

	//��firstBlock�в���tableName�ĵ�һ���ַ
	for (i = firstBlock.begin(); i != firstBlock.end(); i ++)
	{
		if (tableName == i->first)
		{
			break;
		}
	}
	if (i == firstBlock.end())
	{
		// û�ҵ������һ��
		firstBlock.insert(pair<string, int>(block.tableName, block.offset));
	}

	//����LastOffset�������һ�飬��������nextoffsetΪ�¿��offset������dirty
	for (i = lastBlock.begin(); i != lastBlock.end(); ++i)
	{
		if (tableName == i->first)
		{
			break;
		}
	}
	if (i == lastBlock.end())
	{
		// û�ҵ������һ��
		lastBlock.insert(pair<string, int>(block.tableName, block.offset));
	}
	else
	{
		Block& temp = findBlock(i->second);
		temp.nextOffset = block.offset;
		temp.dirty();
		writeBlock(temp);
	}
	/*- - -*/
	int offset = block.offset;
	return findBlock(offset);
}

void BufferManager::debug(bool isContent = false)
{
	cout << "BUFFER" << endl;
	for (list<Block>::iterator i = buffer.begin(); i != buffer.end(); ++i)
	{
		i->debug(isContent);
	}
	cout << "END" << endl;
}

/* ���²��ּ������� */

Block& BufferManager::getBlocks(int offset)
{
	return findBlock(offset);
}

void BufferManager::storeBlocks(int offset, Block& block)
{
	block.offset = offset;
	writeBlock(block);
}


vector<int> BufferManager::getIndexBlocks(string indexName)
{
	return getTableBlocks("!" + indexName);
}

Block& BufferManager::newIndexBlock(string indexName)
{
	Block& b = newBlock("!" + indexName);
	b.isIndex = true;
	return b;
}

Block& BufferManager::getIndexOneBlock(string indexName, int offset)
{
	return getBlocks(offset);
}

void BufferManager::writeIndexData(string indexName, char* content, int length)
{
	for (int i = 0, j = 0; i < length; i += j)
	{
		j = (i > BLOCK_LEN) ? BLOCK_LEN : i;
		Block& b = newIndexBlock(indexName);
		memcpy(b.content, content + i, j);
		b.dirty();
	}
}

void BufferManager::writeIndexData(string indexName, int offset, char* content, int length)
{
	Block b = findBlock(offset);
	memcpy(b.content, content, length);
	b.dirty();
}
