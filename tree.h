#ifndef _TREE_H
#define _TREE_H
#include "corrstar.h"
#include "timer.h"

typedef struct TREE_NODE
{
	TIMER timer;
	struct TREE_NODE * pstParent;
	struct TREE_NODE * pstLeftChild;
	struct TREE_NODE * pstRightChild;

}TREE_NODE;

BOOL InsertTree(
	TREE_NODE * pstTempRoot, 
	TREE_NODE * pstNode);

TREE_NODE * DeleteSmallestNode();

TREE_NODE * SearchTree(TREE_NODE * ,PVOID pvData);

#endif