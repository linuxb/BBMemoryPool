/*内存位的链表以及二叉搜索树结构*/
#ifndef _CTREELINKLIST_H_
#define _CTREELINKLIST_H_
#endif

#include "stdafx.h"

#define MAX_NODE 1024
#define NODE_MEMORY 640
#define INCREMENT 2
#define MAX_MEMORY_REQUIRE 1000000


/*空间配置器*/

class TAllocator
{
public:
	TAllocator(){}
	~TAllocator(){}

	static inline void* Allocate(size_t size)
	{
		std::cout << "Allocate " << size << "B memory totally\n" << std::endl;
		return ::operator new(size, ::std::nothrow);
	}

	static inline void deAllocate(void* ptr, size_t size)
	{
		::operator delete(ptr, ::std::nothrow);
		std::cout << "free " << size << "B memory totally\n" << std::endl;
	}

private:

};

struct TreeMemListNode
{
	void* _pMemory;		//内存块指针
	TreeMemListNode* _pNext = NULL;
	/*析构*/
	/*防止内存泄露*/
	~TreeMemListNode()
	{
	}
};

/*avl Tree*/
struct TreeNode
{
	size_t _capacity;
	TreeMemListNode* RHeader = NULL;	//回收链表头指针
	TreeMemListNode* RTail = NULL;	//尾指针
	size_t listNum = 0;

	/*回溯节点指针*/
	TreeNode* _pThr = NULL;

	/*当前树高度*/
	int _height;

	/*children*/
	TreeNode* _LChild = NULL;
	TreeNode* _RChild = NULL;

	/*析构*/
	~TreeNode()
	{
		if (RHeader != NULL)
		{
			TreeMemListNode* pcurr = RHeader;
			TreeMemListNode* ppre;
			while (pcurr != NULL)
			{
				ppre = pcurr;
				pcurr = pcurr->_pNext;
				delete ppre;
			}
		}
	}
};

/*linklist*/
struct LinkNode
{
	void* _pMemory;
	size_t _mUsedByte;
	LinkNode* _next = NULL;
	size_t _capacity;
	size_t _itemSize;
	void* _pAlloateInit = NULL;

	LinkNode(size_t size)
	{
		if (size > MAX_NODE)
		{
			throw std::invalid_argument("size is too large,no more memory");
		}
		/*Allocate*/
		_pMemory = TAllocator::Allocate(size);
		if (_pMemory == NULL) throw std::bad_alloc();
		_mUsedByte = 0;
		_capacity = size;
		_pAlloateInit = _pMemory;
	}

	~LinkNode()
	{
		TAllocator::deAllocate(_pMemory, _capacity);
	}
};
