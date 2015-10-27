/*内存位的链表以及二叉搜索树结构*/
#ifndef _CTREELINKLIST_H_
#define _CTREELINKLIST_H_
#endif

#include "stdafx.h"

#define MAX_NODE 1024
#define NODE_MEMORY 640
#define MAX_ITEM_SIZE 16


/*空间配置器*/

class TAllocator
{
public:
	TAllocator(){}
	~TAllocator(){}

	static inline void* Allocate(size_t size)
	{
		return ::operator new(size, ::std::nothrow);
	}

	static inline void deAllocate(void* ptr, size_t size)
	{
		::operator delete(ptr, ::std::nothrow);
	}

private:

};

/*Binary Tree*/
struct TreeNode
{
	void* _pMemory;		//内存块指针
	size_t _mUsedByte;	//已用内存
	size_t _capacity;
	size_t _mIdx;		//内存位

	/*children*/
	TreeNode* _LChild = NULL;
	TreeNode* _RChild = NULL;
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

	LinkNode(size_t size,size_t itemsize)
	{
		if (size > MAX_NODE)
		{
			throw std::invalid_argument("size is too large,no more memory");
		}
		if (itemsize > MAX_ITEM_SIZE)
		{
			throw std::invalid_argument("itemSize is too large");
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

/*回收链表*/
struct RecycleLinkNode
{
	void* _pRecycledMemory = NULL;
	size_t _mSize;

	RecycleLinkNode* _next = NULL;
};