/////////////////////////////////////////////////////////////////////////
//																	   //
//																	   //
// COPYRIGHT NOTICE                                                    //
// Copyright (c) 2014                                                  //
//                                                                     //
// @File Name: tree.cpp                                                //
// @File Function: Sort Binary Tree         		                   //
//																	   //
// @Version 1.0 													   //
// @Author Duan Kai													   //
// @Date 2014-06-25													   //
//																	   //
/////////////////////////////////////////////////////////////////////////

#include "tree.h"


TREE_NODE * g_pstRoot = NULL;

TREE_NODE * g_pstSmallestNode = NULL;

/////////////////////////////////////////////////////////////////////////
//
// @Function: Create the sort binary tree
//
/////////////////////////////////////////////////////////////////////////

BOOL g_UPDATE = FALSE;

BOOL InsertTree(
	TREE_NODE * pstTempRoot, 
	TREE_NODE * pstNode)
{
	if (NULL == pstNode)
	{
		return FALSE;
	}

	if (NULL != pstTempRoot)
	{
		if (pstNode->timer.lMilliSecond > pstTempRoot->timer.lMilliSecond)
		{
			if (NULL != pstTempRoot->pstRightChild)
			{
				InsertTree(pstTempRoot->pstRightChild, pstNode);
			}
			else
			{
				pstTempRoot->pstRightChild = pstNode;
				pstNode->pstParent = pstTempRoot;
			}
		}
		else
		{
			if (pstTempRoot == g_pstRoot)
			{
				g_UPDATE = TRUE;
			}
			if (NULL != pstTempRoot->pstLeftChild)
			{
				InsertTree(pstTempRoot->pstLeftChild, pstNode);
			}
			else
			{
				pstTempRoot->pstLeftChild = pstNode;
				pstNode->pstParent = pstTempRoot;
				if (g_UPDATE)
				{
					g_pstSmallestNode = pstNode;
					g_UPDATE = FALSE;
				}
			}
		}
	}
	else
	{
		g_pstRoot = pstNode;
		g_pstSmallestNode = pstNode;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////
//
// @Function: Output the smallest node from the sort binary tree
//
/////////////////////////////////////////////////////////////////////////
TREE_NODE * DeleteSmallestNode()
{
	if (NULL == g_pstSmallestNode)
	{
		return NULL;
	}
		
	TREE_NODE * pstResultNode;
	pstResultNode = g_pstSmallestNode;
	
	if (NULL == g_pstSmallestNode->pstRightChild) 
	{
		if (g_pstRoot != g_pstSmallestNode)
		{
			g_pstSmallestNode = g_pstSmallestNode->pstParent;
		}
		else
		{
			g_pstSmallestNode = NULL;
		}
		return pstResultNode;
	}
	else
	{
		if (g_pstSmallestNode != g_pstRoot)
		{
			g_pstSmallestNode->pstParent->pstLeftChild = g_pstSmallestNode->pstRightChild;
			g_pstSmallestNode->pstRightChild->pstParent = g_pstSmallestNode->pstParent;
		}
		else
		{
			g_pstRoot = g_pstSmallestNode->pstRightChild;
		}

		g_pstSmallestNode = g_pstSmallestNode->pstRightChild;
		while(g_pstSmallestNode->pstLeftChild)
		{
			g_pstSmallestNode = g_pstSmallestNode->pstLeftChild;
		}
		return pstResultNode;
	}
}

TREE_NODE * SearchTree(TREE_NODE * pstRoot, PVOID pvData)
{
	if (NULL == pstRoot || NULL == pvData)
		return NULL;
	if (NULL != pstRoot->pstLeftChild)
		SearchTree(pstRoot->pstLeftChild, pvData);
	if (memcmp(&pstRoot, pvData, sizeof(TIMER)))
	{
		//printf("   uiTimerID:%d,lMilliSecond:%ld\n ",pstRoot->timer.uiTimerID,pstRoot->timer.lMilliSecond);
		return pstRoot;
	}
	
	if(NULL != pstRoot->pstRightChild)
		SearchTree(pstRoot->pstRightChild, pvData);
}

/******************************************************************************************

   test code on the tree.cpp
*******************************************************************************************/
#include "tree.h"
extern TREE_NODE * g_pstRoot;
void print_tree_middle(TREE_NODE * pstRoot)
{
	if(NULL == pstRoot)
		return;
	if(NULL != pstRoot->pstLeftChild)
		print_tree_middle(pstRoot->pstLeftChild);
	/*else
	{
		printf("   uiTimerID:%d,lMilliSecond:%ld\n ",pstRoot->timer.uiTimerID,pstRoot->timer.lMilliSecond);
	}*/
	printf("   uiTimerID:%d,lMilliSecond:%ld\n ",pstRoot->timer.uiTimerID,pstRoot->timer.lMilliSecond);
	if(NULL != pstRoot->pstRightChild)
		print_tree_middle(pstRoot->pstRightChild);
	/*else
		printf("   uiTimerID:%d,lMilliSecond:%ld\n ",pstRoot->timer.uiTimerID,pstRoot->timer.lMilliSecond);*/
}
void print_tree_first(TREE_NODE * pstRoot)
{
	if(NULL != pstRoot)
		printf("   uiTimerID:%d,lMilliSecond:%ld\n ",pstRoot->timer.uiTimerID,pstRoot->timer.lMilliSecond);
	if(NULL != pstRoot->pstLeftChild)
		print_tree_first(pstRoot->pstLeftChild);
	if(NULL != pstRoot->pstRightChild)
		print_tree_first(pstRoot->pstRightChild);
}

	////////////////////////////////////////////////////////////////
	//print_tree_middle(g_pstRoot);

	//printf("===========================\n");

	//print_tree_first(g_pstRoot);

	//////////////////////////////////////////////////////////////
/******************************************************************************************

   test code on the tree.cpp
*******************************************************************************************/