/****************************
Yirat Romano                *
12/02/2024                  *
                       *
source file                 *
exercise: slist             *
*****************************/
#ifndef DLIST_H
#define DLIST_H

#include <stddef.h>

/*
Doubly Linked List Library Documentation

This documentation provides details on the functions available in the Doubly 
Linked List (DList) library. Doubly Linked List is a data structure that 
consists of a sequence of elements, each containing a pointer to the next 
and previous elements in the sequence.
*/

typedef struct dlist dlist_t;
typedef struct dlist_node *dlist_iter_t;  

/*
typedef int (*match_func_t)(const void *data, void *param);

    Description: Defines the function pointer type for matching functions used 
    in searching for elements within a doubly linked list.
    Arguments:
        data: Pointer to the data of the element being evaluated.
        param: Additional parameter provided to the matching function for 
        comparison.
    Returns: An integer value indicating whether the element matches the 
    specified condition.
*/
typedef int (*match_func_t)(const void *data, void *param);

/*
typedef int (*action_func_t)(void *data, void *param);

    Description: Defines the function pointer type for action functions used in 
    performing operations on elements within a doubly linked list.
    Arguments:
        data: Pointer to the data of the element on which the action is 
        performed.
        param: Additional parameter provided to the action function for 
        performing the operation.
    Returns: An integer value indicating the success or failure of the 
    operation.
*/
typedef int (*action_func_t)(void *data, void *param);

/***************************************************************************/
/*	Description: Creates a LinkList.								*/
/*	Complexity: O(1)											*/
/*	Return Value: An adress to the list.							*/
dlist_t *DListCreate();

/***************************************************************************/
/*	Description: Free the allocation made for the list.				*/
/*	Complexity: O(1)											*/
/*	Arguments: list - the list to be freed.							*/
void DListDestroy(dlist_t *list);

/***************************************************************************/
/*	Description: Get the first node in the list.						*/
/*	Complexity: O(1)											*/
/*	Arguments: list - the relevant list							*/
/*	Return Value: An iterator to the first node in the list.			*/
dlist_iter_t DListBegin(const dlist_t *list);

/***************************************************************************/
/*	Description: Get the last node in the list.						*/
/*	Complexity: O(1)											*/
/*	Arguments: list - the relevant list							*/
/*	Return Value: An iterator to the last node in the list.			*/
dlist_iter_t DListEnd(const dlist_t *list);

/***************************************************************************/
/*	Description: Get the next node to your iterator.					*/
/*	Complexity: O(1)			dlist								*/
/*	Arguments: iter - an iterator pointer							*/
/*	Return Value: An iterator to the next node in the list.			*/
dlist_iter_t DListNext(dlist_iter_t iter);

/***************************************************************************/
/*	Description: Get the previous node to your iterator.				*/
/*	Complexity: O(1)											*/
/*	Arguments: iter - an iterator pointer							*/
/*	Return Value: An iterator to the previous node in the list.			*/
dlist_iter_t DListPrev(dlist_iter_t iter);

/***************************************************************************/
/*	Description: Compare the adresses of 2 iterators					*/
/*	Complexity: O(1)											*/
/*	Arguments: iter1 - First iterator pointer						*/
/*		 	 iter2 - Second iterator pointer						*/
/*	Return Value: Returns 1 if the adresses are the same and 0 otherwise	*/
int DListIsIterSame(dlist_iter_t iter1, dlist_iter_t iter2);

/*	Description: Insert a node before the iterator pointer				*/
/*	Complexity: O(1)											*/
/*	Arguments: where - an iterator pointer							*/
/*		  	 data  - the data to be inserted to the node				*/
/*	Return Value: Returns an iterator pointed to the new node			*/
dlist_iter_t DListInsert(dlist_iter_t where, void* data);

/***************************************************************************/
/*	Description: Remove a node pointed by an iterator					*/
/*	Complexity: O(1)											*/
/*	Arguments: to_remove - an iterator pointer to the node needed to		*/
/*		  	 		   be removed, cannot point to end node,		*/
/*					   the next node will be freed				*/
/*	Return Value: Returns an iterator pointed to the new node			*/
dlist_iter_t DListRemove(dlist_iter_t to_remove);

/***************************************************************************/
/*	Description: Get the data stored in the node						*/
/*	Complexity: O(1)											*/
/*	Arguments: iter - an iterator pointer							*/
/*	Return Value: Returns the data from the iterator					*/
void *DListGetData(dlist_iter_t iter);

/***************************************************************************/
/*	Description: Set data to a specific node						*/
/*	Complexity: O(1)											*/
/*	Arguments: iter - an iterator pointer							*/
/*		  	 data - the data to insert into the node				*/
void DListSetData(dlist_iter_t iter, void *data);

/***************************************************************************/
/*	Description: Get the number of nodes in a list					*/
/*	Complexity: O(n)											*/
/*	Arguments: list - the relevant list							*/
/*	Return Value: Returns the number of nodes in the list				*/
size_t DListCount(const dlist_t *list);

/***************************************************************************/
/*	Description: Check if the list is empty							*/
/*	Complexity: O(1)											*/
/*	Arguments: list - the relevant list							*/
/*	Return Value: Returns 1 if the list is empty and 0 otherwise		*/
int DListIsEmpty(const dlist_t *list);

/***************************************************************************/
/*	Description: Run a match function through nodes in a list			*/
/*	Complexity: O(n)											*/
/*	Arguments: from  - first node to test							*/
/*			 to    - last node to test							*/
/*			 match_func_t - a function to run through the nodes		*/
/*			 param - parameter to pass to the function				*/
/*	Return Value: Returns the node where function passed				*/
dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, match_func_t is_match, void *param);

/***************************************************************************/
/*	Description: Run a function through nodes in a list				*/
/*	Complexity: O(n)											*/
/*	Arguments: from  - first node to test							*/
/*			 to    - last node to test							*/
/*			 action - a function to run through the nodes			*/
/*			 param - parameter to pass to the function				*/
/*	Return Value: Return 1 if function succeed and 0 otherwise			*/
int DListForEach(dlist_iter_t from, dlist_iter_t to, action_func_t action, void *param);

/***************************************************************************/
/*	Description: Run a function through nodes in a list				*/
/*	Complexity: O(1)											*/
/*	Arguments: from  - first node to slice							*/
/*			 to    - last node to slice ("to"'s node is excluded)		*/
/*			 where - insert the sliced list before "where"'s node		*/
/*				    "where" iterator must not point to a node within	*/
/*				    the sliced list"							*/
void DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where);

/***************************************************************************/
/*	Description: Run a match function through nodes in a list and inserts */
/*			   all matches to a different list						*/
/*	Complexity: O(n)											*/
/*	Arguments: from  - first node to test							*/
/*			 to    - last node to test							*/
/*			 match_func_t - a function to run through the nodes		*/
/*			 param - parameter to pass to the function				*/
/*	Return Value: Returns 0 if atleast one match is found and -1 otherwise*/
int DListMultiFind(dlist_iter_t from, dlist_iter_t to, match_func_t is_match, void *param, dlist_t *found_elements);

/***************************************************************************/
/*	Description: Push a node to the end of a list.					*/
/*	Complexity: O(1)											*/
/*	Arguments: list - the relevant list							*/
/*		  	 data  - the data to be inserted to the node				*/
/*	Return Value: Returns an iterator pointed to the new node			*/
dlist_iter_t DListPushBack(dlist_t *list, void *data);

/***************************************************************************/
/*	Description: Pop a node from the end of a list.					*/
/*	Complexity: O(1)											*/
/*	Arguments: list - the relevant list							*/
/*	Return Value: Returns an iterator pointed the end of the list		*/
void *DListPopBack(dlist_t *list);

/***************************************************************************/
/*	Description: Push a node to beginning of a list.					*/
/*	Complexity: O(1)											*/
/*	Arguments: list - the relevant list							*/
/*		  	 data  - the data to be inserted to the node				*/
/*	Return Value: Returns an iterator pointed to the new node			*/
dlist_iter_t DListPushFront(dlist_t *list, void *data);

/***************************************************************************/
/*	Description: Pop a node from the beginning of a list.				*/
/*	Complexity: O(1)											*/
/*	Arguments: list - the relevant list							*/
/*	Return Value: Returns an iterator pointed to the new first node		*/
void *DListPopFront(dlist_t *list);


#endif /* DLIST_H */
