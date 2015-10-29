
#ifndef _CMEMORYPOOL_H_
#define _CMEMORYPOOL_H_
#endif

#include "CMemoryManager.h"

/*测试类*/
class Traii
{
public:
	Traii() {}
	Traii(int val) { _val = val; }
	~Traii()
	{
		std::cout << "object destroyed.." << std::endl;
	}

private:
	int _val;
};


class CMemoryPool
{
public:
	~CMemoryPool();
	/*单例*/
	static inline CMemoryPool* getDefaultMemoryPool()
	{
		if (mPool == NULL)
			mPool = new CMemoryPool;
		return mPool;
	}
	void InitPool(size_t nSize = NODE_MEMORY);
	void destroyPool();
	void* getNewMemory(size_t size);

	template<typename T>
	void recyleMemory(T*);
	void* requireMemoryFromRecycle(size_t);

private:
	CMemoryPool();
	static CMemoryPool* mPool;
	/*内存链表*/
	LinkNode* QHeaher = NULL;
	LinkNode* QTail = NULL;
	int mListNodeNums;
	CMemoryManager* pMProxy = NULL;
	void newNode(size_t nSize = NODE_MEMORY);
	/*内嵌私有类*/
	/*RAII技法*/
	class GCWorker
	{
	public:
		/*析构回收单例*/
		~GCWorker()
		{
			if (CMemoryPool::getDefaultMemoryPool() != NULL)
			{
				delete CMemoryPool::mPool;
			}
		}
	};
	/*程序结束时析构函数被调用*/
	static GCWorker mGcWorker;
};
