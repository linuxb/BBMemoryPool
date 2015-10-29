// MemoryPool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CMemoryPool.h"


CMemoryPool::CMemoryPool()
{
}

CMemoryPool::~CMemoryPool()
{
	destroyPool();
}

void CMemoryPool::InitPool(size_t nSize)
{
	if (QHeaher == NULL)
	{
		QHeaher = new LinkNode(nSize);
		if (QHeaher == NULL) throw std::bad_alloc();
		QTail = QHeaher;
		mListNodeNums = 1;
	}
	if (pMProxy == NULL)
	{
		pMProxy = new CMemoryManager;
		if (pMProxy == NULL)
			throw std::bad_alloc();
	}
}

void* CMemoryPool::getNewMemory(size_t size)
{
	void* result = QTail->_pAlloateInit;
	char* addr = (char*)result;
	addr += size;
	QTail->_pAlloateInit = addr;
	return result;
}

/*export*/
/*回收对象*/
template<typename T>
void CMemoryPool::recyleMemory(T* ptr)
{
	/*析构对象*/
	ptr->~T();
	_ASSERT(pMProxy != NULL);
	pMProxy->recycleToPool(ptr);
}

/*获取内存*/
void* CMemoryPool::requireMemoryFromRecycle(size_t size)
{
	_ASSERT(pMProxy != NULL);
	void* result = pMProxy->getMemoryFromRecycledPool(size);
	if (result == NULL)
		return getNewMemory(size);
	else
		return result;
}

void CMemoryPool::newNode(size_t nSize)
{
	if (QTail == NULL || pMProxy == NULL)
	{
		std::cerr << "LinkList should be inited.." << std::endl;
		InitPool(nSize);
		return;
	}
	LinkNode* pnode = new LinkNode(nSize);
	if (pnode == NULL) throw std::bad_alloc();
	QTail->_next = pnode;
	QTail = pnode;
	pMProxy->setMemoryAddr(QTail->_pMemory);
	mListNodeNums++;
}

void CMemoryPool::destroyPool()
{
	if (QHeaher == NULL) return;
	LinkNode* p = QHeaher;
	LinkNode* q = p;
	while (p->_next != NULL)
	{
		p = p->_next;
		delete q;
		q = p;
	}
	delete q;
}

void MemoryFactory(int num)
{
	std::vector<Traii*> objList;
	for (int i = 0; i < 10; i++)
	{
		Traii* myobj = (Traii*)CMemoryPool::getDefaultMemoryPool()->requireMemoryFromRecycle(sizeof(Traii));
		objList.push_back(myobj);
	}
	while (!objList.empty())
	{
		Traii* objToRecycle = objList.back();
		objList.pop_back();
		CMemoryPool::getDefaultMemoryPool()->recyleMemory(objToRecycle);
	}
}

///测试内存池

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

