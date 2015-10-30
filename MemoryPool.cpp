// MemoryPool.cpp : 定义控制台应用程序的入口点。
//

#include "CMemoryPool.h"

/*定义式声明*/
CMemoryPool* CMemoryPool::mPool;

CMemoryPool::CMemoryPool()
{
	/*初始化内存池*/
	InitPool();
}

CMemoryPool::~CMemoryPool()
{
	/*销毁内存池*/
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
	///往内存高地址方向
	addr += size;
	/*检查指针越界*/
	if (addr - QTail->_pMemory >= QTail->_capacity)
	{
		/*越界*/
		/*回收内存*/
		_ASSERT(pMProxy != NULL);
		std::cout << "no more space at current node,allocate new node...\n" << std::endl;
		pMProxy->recycleToPool(QTail->_capacity - QTail->_mUsedByte, QTail->_pAlloateInit);
		/*分配新节点*/
		newNode();
		return getNewMemory(size);
	}
	QTail->_pAlloateInit = addr;
	QTail->_mUsedByte = addr - QTail->_pMemory;
	std::cout <<"alloacte "<<size<<"B memory at current linknode...\n"<<std::endl;
	return result;
}

/*export*/
/*回收对象*/
///export 外部接口
template<class T>
void CMemoryPool::recyleMemory(T* ptr)
{
	/*析构对象*/
	ptr->~T();
	_ASSERT(pMProxy != NULL);
	pMProxy->recycleToPool(sizeof(T),ptr);
}

/*获取内存*/
///export 外部接口
void* CMemoryPool::requireMemoryFromRecycle(size_t size)
{
	_ASSERT(pMProxy != NULL);
	void* result = pMProxy->getMemoryFromRecycledPool(size);
	if (result == NULL)
	{
		std::cout << "need " << size << "B memory from main block\n" << std::endl;
		return getNewMemory(size);
	}
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
	std::cout << "allocate " << nSize << "B memory to new linknode\n" << std::endl;
	LinkNode* pnode = new LinkNode(nSize);
	if (pnode == NULL) throw std::bad_alloc();
	QTail->_next = pnode;
	QTail = pnode;
	pMProxy->setMemoryAddr(QTail->_pMemory);
	mListNodeNums++;
}

void CMemoryPool::destroyPool()
{
	std::cout << "destroying memoryPool....\n" << std::endl;
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

///测试函数
void MemoryFactory(int num)
{
	std::vector<Traii*> objList;
	CMemoryPool* myPool = CMemoryPool::getDefaultMemoryPool();
	for (int i = 0; i < 10; i++)
	{
		Traii* myobj = (Traii*)myPool->requireMemoryFromRecycle(sizeof(Traii));
		objList.push_back(myobj);
	}
	while (!objList.empty())
	{
		Traii* objToRecycle = objList.back();
		objList.pop_back();
		myPool->recyleMemory(objToRecycle);
	}
}

///内存分配函数接口
void* MyAlloate(size_t size)
{
	/*边界检测*/
	///不能一次性分配内存超过阙值
	if (size >= MAX_MEMORY_REQUIRE)
		throw std::invalid_argument("out of memory threshold!");
	/*获取单例*/
	CMemoryPool* myPool = CMemoryPool::getDefaultMemoryPool();
	return myPool->requireMemoryFromRecycle(size);
}

///内存回收函数接口
template<typename T>
void MyRecycle(T* ptr)
{
	if (ptr == NULL)
		throw std::invalid_argument("the pointer can not be NULL!");
	/*获取单例*/
	CMemoryPool* myPool = CMemoryPool::getDefaultMemoryPool();
	myPool->recyleMemory(ptr);
}

///测试内存池

int _tmain(int argc, _TCHAR* argv[])
{
	Traii* myObj = (Traii*)MyAlloate(sizeof(Traii));
	Sleep(1500);
	MyRecycle(myObj);
	system("pause");
	return 0;
}

