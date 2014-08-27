/**************************************************************************
* FileName: Fibonacci_heap.c											  *
* Author: Henrich Lauko													  *
*																		  *
* File provides implementation of functions to correct Fibonacci heap.	  *
*																		  *
***************************************************************************/


#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#include "Fibonacci_heap.h"

#define FALSE 0
#define TRUE 1

double log2( double n )  
{   
    return log( n ) / log( 2 );  
}

/* Inits empty node with key and sets pointers to NULL */
Node* InitNode(int key)
{
	Node* node = malloc(sizeof(Node));

	node->child = NULL;
	node->parent = NULL;

	//is sibling to itself
	node->nextNode = node;
	node->prevNode = node;

	node->degree = 0;
	node->flag = 0;

	//set key
	node->key = key;

	return node;
}

/* Creates empty heap */
FibHeap* MakeHeap()
{
	//allocates heap
	FibHeap* heap = malloc(sizeof(FibHeap));

	//initializes empty heap
	heap->minimum = NULL;
	heap->numberOfElements = 0;
	heap->numberOfRoots = 0;

	return heap;
}

/* Destroys subtree with root in node */
void DestroySubTree(FibHeap* heap, Node* node)
{
	if (node != NULL)
	{
		//destroy children
		if (node->child != NULL)
		{
			DestroySubTree(heap, node->child);
		}

		//destroy siblings
		while (node->nextNode != node)
		{
			Node* toRemove = node->nextNode;
			//set new pointers and remove next sibling
			node->nextNode = node->nextNode->nextNode;
			node->nextNode->nextNode->prevNode = node;
			toRemove->nextNode = toRemove;
			toRemove->prevNode = toRemove;
			DestroySubTree(heap, toRemove);
		}

		//set parent pointers
		if (node->parent != NULL)
		{
			node->parent->child = NULL;
		}
		else
		{
			//is root
			heap->numberOfRoots--;
		}

		free(node);
	}
}

/* Destroys all allocated space of heap */
void DestroyHeap(FibHeap* heap)
{
	//destroy heap content
	DestroySubTree(heap, heap->minimum);

	//destroy heap allocated properties
	free(heap);
}

/* Sets pointers in root list of nodes, when inserting new node */
void InsertNodeToRootList(FibHeap* heap, Node* node)
{
	//pointers from new node
	node->prevNode = heap->minimum;
	node->nextNode = heap->minimum->nextNode;
	//pointers to new node (from old nodes)
	heap->minimum->nextNode->prevNode = node;
	heap->minimum->nextNode = node;
}

/* Insterts new key to heap */
void InsertFibHeap(FibHeap* heap, int key)
{
	//creates new node
	Node* newNode = InitNode(key);

	//checking heap emptiness
	if (heap->minimum == NULL)
	{
		//inserting to empty heap
		heap->minimum = newNode;
	}
	else
	{
		//inserting to rootlist
		InsertNodeToRootList(heap, newNode);
		//checking minimum
		if (newNode->key < heap->minimum->key)
		{
			heap->minimum = newNode;
		}
	}
	//increamenting number of elements
	heap->numberOfElements++;
	//update numbre of roots
	heap->numberOfRoots++;
}

/* Inserts all children of node to heap rootlist*/
void InsertSonsToRootList(FibHeap* heap, Node* node)
{
	Node* selectedChild = node->child;
	
	//iterate over all children
	do{
		Node* nextNode = selectedChild->nextNode;
		selectedChild->prevNode->nextNode = NULL;
		//inserts them to rootlist
		InsertNodeToRootList(heap, selectedChild);
		selectedChild->parent = NULL;
		//update number of roots
		heap->numberOfRoots++;
		//update selected node
		selectedChild = nextNode;		
	} while (selectedChild != NULL);
}

/* Merges two nodes from heap root list */
void MergeNodes(FibHeap* heap, Node* firstNode, Node* secondNode)
{
	//firstNode must have lower key
	if (firstNode->key > secondNode->key) //if not exchange them
	{
		Node* tmp = firstNode;
		firstNode = secondNode;
		secondNode = tmp;
	}

	//extract secondNode from root list
	secondNode->prevNode->nextNode = secondNode->nextNode;
	secondNode->nextNode->prevNode = secondNode->prevNode;

	//add seconNode as child to firstNode
	secondNode->parent = firstNode;
	if (firstNode->child == NULL)
	{
		firstNode->child = secondNode;
		secondNode->prevNode = secondNode;
		secondNode->nextNode = secondNode;
	}
	else
	{
		secondNode->prevNode = firstNode->child;
		secondNode->nextNode = firstNode->child->nextNode;
		secondNode->nextNode->prevNode = secondNode;
		secondNode->prevNode->nextNode = secondNode;
	}

	//update degree
	firstNode->degree++;
	//update flags
	secondNode->flag = FALSE;
	//update number of roots
	heap->numberOfRoots--;
}

/* Finds minimum and reduce number of trees in fibonacci heap */
void ConsolidateHeap(FibHeap* heap)
{
	//auxiliary array that holds information about roots
	int arraySize = (int)(2 * log2(heap->numberOfElements) + 0.5);
	Node** roots = (Node**)malloc(arraySize * sizeof(Node*)); //should be large as largest degree id heap, proven not to be bigger than 2*log2(n)
	memset(roots, 0, arraySize * sizeof(Node*));

	Node* selectedNode = heap->minimum;
	int originalNumberOfRoots = heap->numberOfRoots;
	//iterate over all nodes in root list
	for (int i = 0; i < originalNumberOfRoots; i++)
	{
		int degree = selectedNode->degree;
		//if there exists any node with same degree merge them
		while (roots[degree] != NULL)
		{
			Node* mergeNode = roots[degree]; // node to be merged with selected node
		
			MergeNodes(heap, selectedNode, mergeNode); //merge nodes
			
			roots[degree] = NULL; //update roots table
			degree++; 
		}
		roots[degree] = selectedNode; //set flag that node with degree exists
		
		selectedNode = selectedNode->nextNode; //move over nodes
	}

	//find new minimum
	heap->minimum = NULL;
	//iterates over new root list and find the smallest key
	for (int i = 0; i < arraySize; i++)
	{
		if (roots[i] != NULL)
		{
			if ((heap->minimum == NULL) || (roots[i]->key < heap->minimum->key))
			{
				heap->minimum = roots[i];
			}
		}
	}
}

/* Extracts min from heap and finds new min with heap rebuild*/
int ExtractMinFibHeap(FibHeap* heap)
{
	Node* minimum = heap->minimum;

	if (minimum != NULL)
	{
		if (minimum->child != NULL)
		{
			//transfer children to rootlist
			InsertSonsToRootList(heap, minimum);
		}		

		//extraxt min from list
		minimum->prevNode->nextNode = minimum->nextNode;
		minimum->nextNode->prevNode = minimum->prevNode;
		//update number of roots and elements
		heap->numberOfRoots--;
		heap->numberOfElements--;
		//set new minimum
		if (minimum == minimum->nextNode)
		{
			heap->minimum = NULL; //heap had only one node
		}
		else
		{
			heap->minimum = minimum->nextNode;
			ConsolidateHeap(heap); //rebuild heap and find new minimum
		}
	}
	
	int key = minimum->key;	
	free(minimum);
	return key;
}

/* Cuts node from list of parents children and adds node to list of roots */
void HeapCut(FibHeap* heap, Node* node, Node* parent)
{
	//remove node from parents children
	if (node->nextNode == node)
	{
		parent->child = NULL; //is only child
	}
	else
	{
		node->prevNode->nextNode = node->nextNode;
		node->nextNode->prevNode = node->prevNode;
		parent->child = node->nextNode;
	}
	parent->degree--;

	//add node to list of roots
	node->nextNode = heap->minimum->nextNode;
	node->prevNode = heap->minimum;
	node->prevNode->nextNode = node;
	node->nextNode->prevNode = node;
	//update number of roots
	heap->numberOfRoots++;
	//set node properties
	node->parent = NULL;
	node->flag = FALSE;
}

/* Minimalize size of heap */
void HeapCascadingCut(FibHeap* heap, Node* node)
{
	Node* parent = node->parent;
	if (parent != NULL)
	{
		//check flag on node
		if (node->flag == FALSE)
		{
			node->flag = TRUE;
		}
		else
		{
			//if flag is true make cut
			HeapCut(heap, node, parent);
			HeapCascadingCut(heap, parent);
		}
	}
}

/* Decreases key of selected node to new key */
void DecreaseKeyFibHeap(FibHeap* heap, Node* node, int key)
{
	if (node != NULL)
	{
		if (key > node->key)
		{
			return;//DO NOTHING - CANT INCREASE KEY
		}
		else
		{
			//update key
			node->key = key;
			Node* parent = node->parent;

			//update correctnes of the heap
			if ((parent != NULL) && (node->key < parent->key))
			{
				HeapCut(heap, node, parent);
				HeapCascadingCut(heap, parent);
			}
			
			//update minimum
			if (node->key < heap->minimum->key)
			{
				heap->minimum = node;
			}
		}
	}
}

/* Deletes node from heap */
void DeleteFibHeap(FibHeap* heap, Node* node)
{
	// Decrease nodes key to "neagtive infinity"
	DecreaseKeyFibHeap(heap, node, INT_MIN);
	// Extract the negative infinity from heap
	ExtractMinFibHeap(heap);
}


