
#include "CMemoryManager.h"



CManagerTree::CManagerTree()
{
}

CManagerTree::~CManagerTree()
{
}

CMemoryManager::CMemoryManager()
{
}

CMemoryManager ::~CMemoryManager()
{
}

void CMemoryManager::InitMamager(const size_t mkey[], int length, const size_t size)
{
	CreateTree(mkey, length, size);
}

//TODO: 二叉搜索树插入节点
bool CManagerTree::xInsertNode(TreeNode* pre, const size_t element)
{
	/*空树*/
	if (pre == NULL)
	{
		pre = new TreeNode;
		if (pre == NULL) throw std::bad_alloc();
		pre->_pMemory = NULL;
		pre->_mUsedByte = 0;
		pre->_capacity = MEMORY_UNIT_SIZE;
		pre->_mIdx = element;
		return true;
	}
	if (pre->_mIdx == element)
	{
		throw std::invalid_argument("key of BST can not be equal");
		exit(1);
	}
	/*二叉树递归*/
	if (pre->_mIdx > element) return xInsertNode(pre->_LChild, element);
	else return xInsertNode(pre->_RChild, element);
}



//TODO: 二叉树前序遍历
bool CManagerTree::xTraveral(TreeNode* pnode,const void(*func)(TreeNode*))
{
	if (pnode == NULL)
	{
		std::cout << "NULL Tree...Constructing..." << std::endl;
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

//树节点初始内存块分配
void CMemoryManager::TInitMemoryUnitForNode(TreeNode* pnode)
{
	if (pnode == NULL)
		throw std::invalid_argument("Pointer can not be null");
	char* addr = (char*)pMemory;
	addr += pnode->_mIdx;
	pnode->_pMemory = addr;
}

//TODO: 二叉树构建
void CManagerTree::CreateTree(const size_t mkey[], int length,const size_t size)
{
	for (int i = 0; i < length; i++)
	{
		xInsertNode(pManagerTreeRoot, mkey[i]);
	}
}