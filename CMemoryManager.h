/*内存池的管理类*/

#ifndef _CMEMORYMANAGER_H_
#define _CMEMORYMANAGER_H_
#endif

#include "CTreeLinkList.h"

#define MAX_LIST_KEY 6
#define MEMORY_UNIT_SIZE 4

class CManagerTree
{
public:
	CManagerTree();
	virtual ~CManagerTree();
	/*构造二叉树*/
	TreeNode* getMemoryPtr() { return pManagerTreeRoot; }
protected:
	void xInit();
	bool xTraveral(TreeNode*&,const void(*)(TreeNode*));
	TreeNode* xInsertNode(TreeNode*, const size_t,void*,TreeNode*);
	void* xSearchNode(const size_t,TreeNode*);
	bool xCheckTreeBalance(TreeNode*&,int);	//检查avl树是否平衡
	TreeNode* xReconstructTree(TreeNode*&, size_t, int);	//调整树形
	TreeNode* pManagerTreeRoot = NULL;	//管理树根节点指针
	size_t xDepth;
	TreeNode* xCurrNode = NULL;
private:
	TreeNode* DoubleRoatateTree(TreeNode*&, int);	//树双旋转
	TreeNode* SingleRotateTree(TreeNode*&, int);	//树单旋转
	void DestroyedTree(TreeNode*&);	//析构时销毁树
};

class CMemoryManager : public CManagerTree
{
public:
	CMemoryManager ();
	~CMemoryManager ();
	void InitMamager(int, const size_t size = MEMORY_UNIT_SIZE);
	void setMemoryAddr(void* pMemory) { this->pMemory = pMemory; }
	void recycleToPool(size_t,void*);
	void* getMemoryFromRecycledPool(size_t);
private:
	void* pMemory;	//当前链表节点内存区起始地址
};