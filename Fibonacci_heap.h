#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

/**************************************************************************
 * FileName: Fibonacci_heap.h											  *
 * Author: Henrich Lauko												  *
 *																		  *
 * File provides definitions of functions to correct Fibonacci heap.	  *
 *																		  *
 **************************************************************************/

/* Fibonacci node data structure. */
typedef struct Node
{
	struct Node* parent; //pointer to parent, NULL if does not exist
	struct Node* child;  //pointer to child, NULL if does not exist
	struct Node* prevNode; //pointer to previous sibling on the same level
	struct Node* nextNode; //pointer to next sibling on the same level

	int degree; //number of children
	int flag; //binary flag for heap consolidation
	
	int key; //key of the node
}Node;

/* Fibonacci heap data structure */
typedef struct FibHeap
{
	struct Node* minimum; //pointer to minimal element
	int numberOfElements; //number of elements in heap
	int numberOfRoots; //number of roots in rootlist
}FibHeap;

/* Creates empty heap */
/* Returns empty heap with initialized properties */
FibHeap* MakeHeap();

/* Destroys all allocated space of heap */
void DestroyHeap(FibHeap* heap);

/* Insterts new key to heap */
void InsertFibHeap(FibHeap* heap, int key);

/* Extracts min from heap */
/* Returns min key */
int ExtractMinFibHeap(FibHeap* heap);

/* Decreases key of selected node to new key*/
/* If key is bigger than the old one nothing happens */
void DecreaseKeyFibHeap(FibHeap* heap, Node* node, int key);

/* Deletes node from heap */
void DeleteFibHeap(FibHeap* heap, Node* node);

#endif
