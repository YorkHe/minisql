#define BLOCK_SIZE 4096

#define BLOCK_NUMBER 1000

class buffer { //缓存块类，一个对象表示一个块

private:

	string file_name;//所属文件名
	bool is_written;//是否被修改过
	bool is_locked;//是否被锁定
	bool is_data_exist;//是否有数据
	int block_offset;	//在所属文件中的偏移块数
	int LRU_value;		//LRU_value越大，则越容易被替换
	char values[BLOCK_SIZE + 1];	//块的具体数据, SIZE_BLOCK是一个块的大小
public:

	buffer()；
	string getValues(int begin, int end);//读取该块从begin到end位置的数据，参数是起始位置和终止位置
	char getValues(int index);//读取块的某一位置的数据，参数是位置
};




class BufferManager {

public:

	BufferManager();//初始化对象，也就是初始化多个Buffer对象
	~BufferManager();
private:

	buffer buffer_block[BLOCK_NUMBER];	//BufferManager类的私有成员变量是Buffer类对象数组,BLOCK_NUMBER是一个宏，是缓存区的块数
public:

	void flashBack(int bufferNum);//替换参数是用LRU规则计算出来的最少使用的快，要被替换。

	int getBufferNum(string file_name, int block_offset);
	//索引管理、记录管理向缓存区申请某文件的某块，如果它在缓存区内，则返回在缓存区的位置，
	//如果不存在于缓存区且缓存区不满，则查看缓存区空闲的块，并返回，如果缓存区满，则替换最近最少使用的块
	//然后缓存管理从文件区读取申请的某文件的某块，到空闲或被替换的位置，最终返回申请的文件的某块的位置

	void readBlock(string file_name, int block_offset, int buffer_num);
	//缓存区从DB Files读取数据，参数：buffer_num(读取目的块），file_name(读取文件源),block_offset(读取文件源的具体位置）

	int getFreeBuffer();//返回缓存区空闲的块的位置，如果缓冲区满时，则选择LRU大的块
	
	int getFreeBuffer(int file_name);///读取该块从begin到end位置的数据，参数是起始位置和终止
位置

	int checkInBuffer(string file_name, int block_offset);//判断文件的某一块是否在缓存区内

	void writeAll(string filename,int file_buffer_number);//表格文件和索引文件都可以归为普通文件，参数是文件名，和文件的块数
	
	void updateLRU(int buffer_number);//当访问缓存区的某块后，就需要调用这个函数，用来更新LRU.

        void setWritten(int buffer_number);//当修改过缓存区某块后，就需要调用这个函数，来修改buffer的is_written 属性为1,void setWritten()会调用void updateLRU(),因为修改也是一种访问行为

        void setLocked(int buffer_number);//大程要求锁定缓存区的页

        void setNotDataExist(int file_name); //如果一个文件被删除，则相应地在缓存区内懒惰删除，标记缓存区内属于该文件的块的is_data_exist为0，即不存在
	


};


