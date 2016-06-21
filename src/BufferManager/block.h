/**
 * @file   block.h
 * @brief  缂撳啿鏁版嵁鍧�
 * @author tgmerge
 * 
 * 褰撴敹鍒癇ufferManager浼犲洖鐨勫潡鍚庯紝
 * 浣跨敤getContent鏂规硶寰楀埌鍐呴儴鏁版嵁锛堝瓧绗︿覆鏁扮粍锛夋寚閽堬紱
 * 浣跨敤getSize鏂规硶寰楀埌鍐呴儴鏁版嵁鐨勬湁鏁堥暱搴�
 */

/*
鏂囦欢涓殑鍧楃粨鏋�
[   HEAD   ][          CONTENT          ]
HEAD_LEN   64   HEAD鐨勯暱搴�
BLOCK_LEN  4096  CONTENT鐨勯暱搴�

HEAD缁撴瀯
TABLE_NAME      char*  32    璇ュ潡鐨勮〃鍚�
OFFSET			int    4     鍋忕Щ閲�
NEXT_OFFSET     int    4     涓嬩竴鍧楃殑鍋忕Щ閲�
CONTENT_SIZE    int    4     鍧楀唴鏈夋晥鏁版嵁鐨勯暱搴�
IS_ALIVE        bool   1     璇ュ潡鏄惁宸茶鍒犻櫎
IS_INDEX        bool   1     鏄惁鏄痠ndex

CONTENT缁撴瀯
CONTENT         char*  4096  鏁版嵁
*/


#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

//鍧楀唴鏁版嵁鏈€澶у昂瀵�
#define BLOCK_LEN 4096
//鍧楀ご灏哄
#define HEAD_LEN  64

class Block {

public:						// 鍧楀ご鏁版嵁
	char tableName[32];			// 琛ㄥ悕
	int nextOffset;             // 涓嬩竴鍧楀湪鏂囦欢涓殑鍋忕Щ閲�
	int contentSize;			// 鍧楀唴鏈夋晥鏁版嵁闀垮害
	bool isAlive;               // 璇ュ潡鏄惁宸茶鍒犻櫎
	bool isIndex;
public:						// 鍧楁暟鎹�
	char content[BLOCK_LEN];	// 鏁版嵁
public:						// 鍏朵粬
	int offset;					// 璇lock鍦ㄦ枃浠朵腑鐨勫亸绉婚噺
	bool isDirty;				// 鏄惁闇€瑕佸啓鍥炴枃浠�
	//bool isActive;				// for lru
	//int value;					// for lru

public:							// 鏋勯€狅紝鏋愭瀯
	// 鍒濆鍖栦竴涓柊鍧楋紝涓嶆槸绱㈠紩
	Block();
	// 鎸囧畾鏄惁绱㈠紩锛屽垵濮嬪寲涓€涓柊鍧�
	Block(bool index);
	// 濡傛灉dirty锛屽厛鍐欏洖鏂囦欢鍐嶉攢姣�

public:							// 寤鸿鐢ㄤ笅鍒楁柟娉曡鍐欏睘鎬�
	// 璁剧疆杩欎釜鍧楁槸鑴忕殑锛屼互渚垮啓鍥炴枃浠�
	void dirty();

	// 璇诲彇杩欎釜鍧楀唴閮ㄧ殑鏁版嵁
	char* getContent();

	// 璇诲彇杩欎釜鍧楀唴閮ㄧ殑鏁版嵁闀垮害
	int getSize();

	// 娴嬭瘯鐢�
	void debug(bool withContent = false);
};

#endif