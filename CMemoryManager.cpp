
#include "CMemoryManager.h"



CManagerTree::CManagerTree()
{
}

CManagerTree::~CManagerTree()
{
	/*回收内存*/
	std::cout << "destroying our Manager Tree...\n" << std::endl;
	DestroyedTree(pManagerTreeRoot);
}

CMemoryManager::CMemoryManager()
{
}

CMemoryManager ::~CMemoryManager()
{
	std::cout << "invoking the base class deconstructor..." << std::endl;
}

void CMemoryManager::InitMamager(int length, const size_t size)
{
	std::cout << "Manager starting....." << "\n" << std::endl;
}

int calcHeight(TreeNode* tnode)
{
	if (tnode == NULL) return 0;
	return tnode->_height;
}

int maxHeight(int a, int b)
{
	return (a > b) ? a : b;
}

///TODO: avl树插入节点
TreeNode* CManagerTree::xInsertNode(TreeNode* pre, const size_t element,void* pMemory,TreeNode* pthr)
{
	/*空树*/
	/*构建二叉树节点*/
	if (pre == NULL)
	{
		pre = new TreeNode;
		if (pre == NULL) throw std::bad_alloc();
		pre->_capacity = element;
		/*链表空*/
		/*初始化链表*/
		std::cout << "initializing the memoryList on Treenode...\n" << std::endl;
		if (pre->RHeader == NULL)
		{
			TreeMemListNode* mnode = new TreeMemListNode;
			if (mnode == NULL) throw std::bad_alloc();
			pre->RHeader = mnode;
			pre->RTail = pre->RHeader;
			pre->listNum++;
			pre->RTail->_pMemory = pMemory;
			xCurrNode = pre;
		}
		std::cout << "first node for recycled memory has been built\n" << std::endl;
		///避免递归导致堆栈溢出
		pre->_height = maxHeight(calcHeight(pre->_LChild), calcHeight(pre->_RChild));
		return pre;
	}
	if (pre->_capacity == element)
	{
		if (pre->RHeader == NULL)
		{
			std::cerr << "链表空指针异常" << std::endl;
			throw std::nullptr_t();
		}
		/*链表插入节点*/
		TreeMemListNode* mnode = new TreeMemListNode;
		if (mnode == NULL) throw std::bad_alloc();
		_ASSERT(pre->RTail != NULL);
		pre->RTail->_pNext = mnode;
		pre->RTail = mnode;
		pre->RTail->_pMemory = pMemory;
		pre->listNum++;
		std::cout << "new memory node has been inserted\n" << std::endl;
	}
	/*二叉树递归*/
	if (pre->_capacity > element)
	{
		pre->_RChild = xInsertNode(pre->_RChild, element, pMemory, pre);
		if (!xCheckTreeBalance(pre, 0))
		{
			/*重构树形*/
			std::cout << "reconstructing the managerTree....\n" << std::endl;
			pre = xReconstructTree(pre, element, 0);
		}
	}
	else
	{
		pre->_LChild = xInsertNode(pre->_LChild, element, pMemory, pre);
		if (!xCheckTreeBalance(pre, 1))
		{
			std::cout << "reconstructing the managerTree....\n" << std::endl;
			pre = xReconstructTree(pre, element, 1);
		}
	}
	/*重新计算高度*/
	pre->_height = maxHeight(calcHeight(pre->_LChild), calcHeight(pre->_RChild));
	return pre;
	/*avl树不平衡*/
	/*树旋转保持平衡*/
}

/*检查树是否平衡*/
///flag:1(左子树插入)
bool CManagerTree::xCheckTreeBalance(TreeNode* &tnode,int flag)
{
	int delta = calcHeight(tnode->_LChild) - calcHeight(tnode->_RChild);
	delta = (flag == 1) ? delta : (-delta);
	if (delta == 2) return false;
	return true;
}

/*重建二叉树树形*/
///flag:1左子树插入
TreeNode* CManagerTree::xReconstructTree(TreeNode* &tnode,size_t key,int flag)
{
	/*二叉树不平衡*/
	/*判断构型*/
	_ASSERT(tnode->_LChild != NULL && tnode->_RChild != NULL);
	/*左子树插入*/
	if (flag == 1)
	{
		_ASSERT(key != tnode->_LChild->_capacity);
		if (key < tnode->_LChild->_capacity)
		{
			/*LL型*/
			/*右旋*/
			std::cout << "reconstructing LL...." << std::endl;
			return SingleRotateTree(tnode, 0);
		}
		else
		{
			/*LR型*/
			/*左右旋*/
			std::cout << "reconstructing LR...." << std::endl;
			return DoubleRoatateTree(tnode, 1);
		}
	}
	/*右子树插入*/
	if (flag == 0)
	{
		_ASSERT(key != tnode->_RChild->_capacity);
		if (key < tnode->_RChild->_capacity)
		{
			/*RL型*/
			/*右左旋*/
			std::cout << "reconstructing RL...." << std::endl;
			return DoubleRoatateTree(tnode, 0);
		}
		else
		{
			/*RR型*/
			/*左旋*/
			std::cout << "reconstructing RR...." << std::endl;
			return SingleRotateTree(tnode, 1);
		}
	}
	else
	{
		throw std::invalid_argument("error flag to reconstruct the tree");
		exit(1);
	}
}

/*单旋转*/
///flag:1左旋 flag:0右旋
TreeNode* CManagerTree::SingleRotateTree(TreeNode* &tnode, int flag)
{
	/*右旋*/
	if (flag == 1)
	{
		TreeNode* porigin = tnode->_LChild;
		tnode->_LChild = porigin->_RChild;
		porigin->_RChild = tnode;
		/*重新子树计算高度*/
		std::cout << "recaculate the height of current node...\n" << std::endl;
		tnode->_height = maxHeight(calcHeight(tnode->_LChild), calcHeight(tnode->_RChild));
		porigin->_height = maxHeight(calcHeight(porigin->_LChild),calcHeight(porigin->_RChild));
		return porigin;
	}
	/*左旋*/
	if (flag == 0)
	{
		TreeNode* porigin = tnode->_RChild;
		tnode->_RChild = porigin->_LChild;
		porigin->_LChild = tnode;
		std::cout << "recaculate the height of current node...\n" << std::endl;
		tnode->_height = maxHeight(calcHeight(tnode->_LChild), calcHeight(tnode->_RChild));
		porigin->_height = maxHeight(calcHeight(porigin->_LChild), calcHeight(porigin->_RChild));
		return porigin;
	}
	else
	{
		throw std::invalid_argument("error flag to reconstruct the tree");
		exit(1);
	}
}

/*双旋转*/
///flag:1左右旋 flag:0右左旋
TreeNode* CManagerTree::DoubleRoatateTree(TreeNode* &tnode, int flag)
{
	/*左右旋*/
	if (flag == 1)
	{
		/*左旋*/
		tnode->_LChild = SingleRotateTree(tnode->_LChild, 1);
		/*右旋*/
		return SingleRotateTree(tnode, 0);
	}
	if (flag == 0)
	{
		/*右旋*/
		tnode->_RChild = SingleRotateTree(tnode->_RChild, 0);
		/*左旋*/
		return SingleRotateTree(tnode, 1);
	}
	else
	{
		throw std::invalid_argument("error flag to reconstruct the tree");
		exit(1);
	}
}

//TODO: 二叉树前序遍历
bool CManagerTree::xTraveral(TreeNode* &pnode,const void(*func)(TreeNode*))
{
	if (pnode == NULL)
	{
		std::cout << "NULL Tree...Constructing...\n" << std::endl;
		return false;
	}
	if (pnode->_LChild == NULL)
	{
		func(pnode);
		if (pnode->_RChild != NULL) xTraveral(pnode->_RChild, func);
		else return true;
	}
	/*左子树递归*/
	else xTraveral(pnode->_LChild, func);
	_ASSERT(pnode != NULL);
	func(pnode);
	/*右子树递归*/
	if (pnode->_RChild != NULL) xTraveral(pnode->_RChild, func);
	else return true;
}

/*查找回收池*/
///从回收池中获取内存
void* CManagerTree::xSearchNode(size_t size,TreeNode* tnode)
{
	std::cout << "searching for available memory node....\n" << std::endl;
	/*回收树空或没有对应内存节点*/
	if (tnode == NULL)
		return NULL;
	if (size == tnode->_capacity)
	{
		/*从链表获取内存指针*/
		if (tnode->RTail == NULL)
			/*无复用内存*/
			return NULL;
		/*前驱指针*/
		TreeMemListNode* proip = tnode->RHeader;
		while (proip->_pNext != tnode->RTail)
			proip = proip->_pNext;
		void* result = tnode->RTail->_pMemory;
		delete tnode->RTail;
		proip->_pNext = NULL;
		tnode->RTail = proip;
		tnode->listNum--;
		std::cout << "get " << size << "B memory\n" << std::endl;
		return result;
	}
	/*右子树递归*/
	if (size > tnode->_capacity)
		return xSearchNode(size, tnode->_RChild);
	/*左子树递归*/
	else
		return xSearchNode(size, tnode->_LChild);
}

/*export*/
/*向回收树添加节点*/
void CMemoryManager::recycleToPool(size_t size,void* ptr)
{
	std::cout << "recycle " <<size<<"B memory\n"<<std::endl;
	pManagerTreeRoot = xInsertNode(pManagerTreeRoot, size, ptr, NULL);
}

/*在回收树中获取内存*/
void* CMemoryManager::getMemoryFromRecycledPool(size_t size)
{
	std::cout << "search " << size << "B memory\n" << std::endl;
	return xSearchNode(size, pManagerTreeRoot);
}

/*销毁树*/
void CManagerTree::DestroyedTree(TreeNode* &tnode)
{
	if (tnode == NULL)
		return;
	/*左子树*/
	DestroyedTree(tnode->_LChild);
	/*右子树*/
	DestroyedTree(tnode->_RChild);
	delete tnode;
}
