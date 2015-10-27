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

void CMemoryPool::InitPool(size_t nSize, size_t itemSize)
{
	if (QHeaher == NULL)
	{
		if (nSize % itemSize)
		{
			std::cerr << "invalid arguments.." << std::endl;
			nSize = (nSize / itemSize) * itemSize;
		}
		QHeaher = new LinkNode(nSize, itemSize);
		if (QHeaher == NULL) throw std::bad_alloc();
		QTail = QHeaher;
		mListNodeNums = 1;
		pMProxy = new CMemoryManager;
		pMProxy->setMemoryAddr(QTail->_pMemory);
		/*构造管理二叉树*/
		size_t length = nSize / itemSize;
		size_t mKeyArr[MAX_NODE / MEMORY_UNIT_SIZE];
		for (unsigned int i = 1; i <= length; i++)
		{
			mKeyArr[i - 1] = itemSize * i;
		}
		/*avl树构造*/
		pMProxy->InitMamager(mKeyArr, length, itemSize);
		/*树不平衡，完成树旋转*/
	}
}

void* CMemoryPool::requireMemoryFromRecycle(size_t size)
{
	if (RHeader == NULL)
		return getNewMemory(size);
	/*检索链表*/
	RecycleLinkNode* pnode = RHeader;
	while (pnode != NULL)
	{
		if (pnode->_mSize >= size)
			break;
		else pnode = pnode->_next;
	}
	if (pnode == NULL)
	{
		std::cout << "no more free memory..need to malloc new block" << std::endl;
		return getNewMemory(size);
	}
	return pnode->_pRecycledMemory;
}

void* CMemoryPool::getNewMemory(size_t size)
{
	void* result = QTail->_pAlloateInit;
	char* addr = (char*)result;
	addr += size;
	QTail->_pAlloateInit = addr;
	return result;
}

void CMemoryPool::addNewRecycleNode()
{
	/*回收链表空*/
	if (RHeader == NULL || RTail == NULL)
	{
		RHeader = new RecycleLinkNode;
		if (RHeader == NULL) throw std::bad_alloc();
		RTail = RHeader;
		return;
	}
	RecycleLinkNode* pnode = new RecycleLinkNode;
	if (pnode == NULL) throw std::bad_alloc();
	RTail->_next = pnode;
	RTail = pnode;
}

template<typename T>
void CMemoryPool::recyleMemory(T* ptr)
{
	/*析构对象*/
	ptr->~T();
	addNewRecycleNode();
	_ASSERT(RTail != NULL);
	RTail->_mSize = sizeof(T);
	RTail->_pRecycledMemory = ptr;
}

void CMemoryPool::newNode(size_t nSize, size_t itemSize)
{
	if (QTail == NULL || pMProxy == NULL)
	{
		std::cerr << "LinkList should be inited.." << std::endl;
		InitPool(nSize, itemSize);
		return;
	}
	LinkNode* pnode = new LinkNode(nSize, itemSize);
	if (pnode == NULL) throw std::bad_alloc();
	QTail->_next = pnode;
	QTail = pnode;
	pMProxy->setMemoryAddr(QTail->_pMemory);
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


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

