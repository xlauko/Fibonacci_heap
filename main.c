/**************************************************************************
* FileName: main.c 														  *
* Author: Henrich Lauko 												  *
* 																		  *
* File provides implementation of tests for Fibonacci heap. 			  *
* 																		  *
***************************************************************************/
#include <stdio.h>
#include "Fibonacci_heap.h"

//macros for testing
#define OK 1
#define FALSE 0

//test functions
int runTests();
int initTest();
int insertSingleKeyTest();
int insertMoreKeysTest();
int extractMinSingleNodeTest();
int extractMinMoreNodesTest();
int decreasKeySingleNodeTest();
int decreasKeyMoreNodesTest();
int deleteNodeSingleNodeTest();
int deleteNodeMoreNodeTest();

int main()
{
	//testing fibonacci heap
	if (runTests() == OK)
	{
		printf("Tests passed! :)\n");
	}
	else
	{
		printf("Tests failed! :(\n");
	}
	return 0;
}

/* Runs all tests */
int runTests()
{
	//Init test
	if (initTest() == FALSE)
	{
		printf("Initialization failed!\n");
		return FALSE;
	}
	
	//Insert tests
	if (insertSingleKeyTest() == FALSE)
	{
		printf("Inserting single node failed!\n");
		return FALSE;
	}	
	if (insertMoreKeysTest() == FALSE)
	{
		printf("Inserting more nodes failed!\n");
		return FALSE;
	}
	
	//extract min test
	if ((extractMinSingleNodeTest() == FALSE) || (extractMinMoreNodesTest() == FALSE))
	{
		printf("Extracting min failed!\n");
		return FALSE;
	}
	
	//decrease key test
	if (decreasKeySingleNodeTest() == FALSE)
	{
		printf("Decreasing key in single node heap failed!\n");
		return FALSE;
	}	
	if (decreasKeyMoreNodesTest() == FALSE)
	{
		printf("Decreasing key in more node heap failed!\n");
		return FALSE;
	}
	
	//delete key test
	if (deleteNodeSingleNodeTest() == FALSE)
	{
		printf("Delete node in single node heap failed!\n");
		return FALSE;
	}	
	if (deleteNodeMoreNodeTest() == FALSE)
	{
		printf("Delete node in more node heap failed!\n");
		return FALSE;
	}
	
	return OK;
}

/* Setup fibonacci heap with elements {1,2,3,4,5} */
FibHeap* setupTest()
{
	FibHeap* heap = MakeHeap();
	
	for (int i = 1; i <= 5; i++)
	{
		InsertFibHeap(heap, i);
	}
	
	return heap;
}

/* Tests heap initialization */
int initTest()
{
	FibHeap* heap = MakeHeap();
	
	//is heap initialized correctly
	if (heap->minimum != NULL)
	{
		 return FALSE;
	}	
	if (heap->numberOfElements != 0)
	{
		return FALSE;
	}
	
	DestroyHeap(heap);
	return OK;
}

/* Tests one node insert to heap */
int insertSingleKeyTest()
{
	FibHeap* heap = MakeHeap();
	InsertFibHeap(heap, 1);
	
	//heap properties
	if (heap->minimum->key != 1)
	{
		printf("Insert test failed: Incorrect minimum.\n");
		return FALSE;
	}
	if (heap->numberOfElements != 1)
	{
		printf("Insert test failed: Wrong number of nodes in heap.\n");
		return FALSE;
	}
	//node properties
	if (heap->minimum->child != NULL) //child
	{
		printf("Insert test failed: Unset node child.\n");
		return FALSE;
	}
	if (heap->minimum->parent != NULL) //parent
	{
		printf("Insert test failed: Unset node parent.\n");
		return FALSE;
	}
	if ((heap->minimum->nextNode != heap->minimum) || (heap->minimum->prevNode != heap->minimum)) //siblings
	{
		printf("Insert test failed: Unset node siblings.\n");
		return FALSE;
	}
	if ((heap->minimum->degree != 0) || (heap->minimum->flag != 0)) //properties - degree and flag
	{
		printf("Insert test failed: Unset node properties.\n");
		return FALSE;
	}
	
	DestroyHeap(heap);
	return OK;
}

/* Tests more insertions to heap */
int insertMoreKeysTest()
{
	FibHeap* heap = setupTest();
	
	//heap properties
	if (heap->minimum->key != 1)
	{
		printf("Insert test failed: Incorrect minimum.\n");
		return FALSE;
	}
	if (heap->numberOfElements != 5)
	{
		printf("Insert test failed: Wrong number of nodes in heap.\n");
		return FALSE;
	}
	
	//node properties
	if (heap->minimum->nextNode->key != 5 )
	{
		printf("Insert test failed: Wrong order of nodes in heap.\n");
		return FALSE;
	}
	if (heap->minimum->prevNode->key != 2)
	{
		printf("Insert test failed: Wrong order of nodes in heap.\n");
		return FALSE;
	}
	
	DestroyHeap(heap);
	return OK;
}

/* Tests single extrac min from heap */
int extractMinSingleNodeTest()
{
	FibHeap* heap = setupTest();
	
	int min = 0;
	min = ExtractMinFibHeap(heap);
	
	if (min != 1)
	{
		printf("Extract min test failed: Incorrect minimum returned.\n");
		return FALSE;
	}
	if (heap->minimum->key != 2)
	{
		printf("Extract min test failed: Incorrect minimum set.\n");
		return FALSE;
	}
	if ((heap->numberOfElements != 4) || (heap->numberOfRoots != 1))
	{
		printf("Extract min test failed: Incorrect heap properies set.\n");
		return FALSE;
	}
	
	DestroyHeap(heap);
	return OK;
}

/* Tests more extractions from heap */
int extractMinMoreNodesTest()
{
	FibHeap* heap = setupTest();
	
	int min = 0;
	ExtractMinFibHeap(heap);
	min = ExtractMinFibHeap(heap);
	
	if (min != 2)
	{
		printf("Extract min test failed: Incorrect minimum returned.\n");
		return FALSE;
	}
	if (heap->minimum->key != 3)
	{
		printf("Extract min test failed: Incorrect minimum set.\n");
		return FALSE;
	}	
	if ((heap->numberOfElements != 3) || (heap->numberOfRoots != 2))
	{
		printf("Extract min test failed: Incorrect heap properies set.\n");
		return FALSE;
	}
	
	DestroyHeap(heap);
	
	return OK;
}

/* Tests decreasing single key */
int decreasKeySingleNodeTest()
{
	FibHeap* heap = MakeHeap();
	
	InsertFibHeap(heap, 10);
	DecreaseKeyFibHeap(heap, heap->minimum, 5);
	
	if (heap->minimum->key != 5)
	{
		printf("Decrease key test failed: Incorrect minimum set.\n");
		return FALSE;
	}
	
	DestroyHeap(heap);
	return OK;
}

/* Tests decreasing single key */
int decreasKeyMoreNodesTest()
{
	FibHeap* heap = setupTest();
	
	ExtractMinFibHeap(heap);
	DecreaseKeyFibHeap(heap, heap->minimum->child, 0);
	
	if (heap->minimum->key != 0)
	{
		printf("Decrease key test failed: Incorrect minimum set.\n");
		return FALSE;
	}
	if (heap->minimum->nextNode->key != 2)
	{
		printf("Decrease key test failed: Incorrect node cut.\n");
		return FALSE;
	}
	if (heap->minimum->nextNode->child->key != 4)
	{
		printf("Decrease key test failed: Incorrect node cut.\n");
		return FALSE;
	}
	
	DestroyHeap(heap);
	return OK;
}

/* Tests deleting node from heap with single node */
int deleteNodeSingleNodeTest()
{
	FibHeap* heap = MakeHeap();
	
	InsertFibHeap(heap, 10);
	DeleteFibHeap(heap, heap->minimum);
	
	if (heap->minimum != NULL)
	{
		printf("Delete node test failed: Incorrect heap minimum setup.\n");
		return FALSE;
	}
	if (heap->numberOfElements != 0 )
	{
		printf("Delete node test failed: Incorrect number of nodes.\n");
		return FALSE;
	}
	if (heap->numberOfRoots != 0)
	{
		printf("Delete node test failed: Incorrect number of roots.\n");
		return FALSE;
	}
	
	DestroyHeap(heap);
	return OK;
}

/* Tests deleting node from heap with more nodes */
int deleteNodeMoreNodeTest()
{
	FibHeap* heap = setupTest();
	DeleteFibHeap(heap, heap->minimum->nextNode);
	
	if (heap->minimum->key != 1)
	{
		printf("Delete node test failed: Incorrect heap minimum setup.\n");
		return FALSE;
	}
	if (heap->numberOfElements != 4)
	{
		printf("Delete node test failed: Incorrect number of nodes.\n");
		return FALSE;
	}
	if (heap->numberOfRoots != 1)
	{
		printf("Delete node test failed: Incorrect number of roots.\n");
		return FALSE;
	}
	if (heap->minimum->child->key != 2)
	{
		printf("Delete node test failed: Incorrect nodes cut.\n");
		return FALSE;
	}
	
	DestroyHeap(heap);
	return OK;
}
