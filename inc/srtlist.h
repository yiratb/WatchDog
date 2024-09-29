/****************************
Yirat Romano                *
20/02/2024                  *
                            *
header file                 *
exercise: slist             *
*****************************/
#ifndef SRTLIST_H
#define SRTLIST_H

#include <stddef.h> /*size_t*/
#include "dlist.h" /*dlist_iter_t*/
      
typedef int (*cmp_func_t)(const void *data, const void *param);

typedef struct srtlist srtlist_t;

typedef struct srtlist_iter
{
    dlist_iter_t iter;
    #ifndef NDEBUG 
    srtlist_t *list;
    #endif 
} srtlist_iter_t;   

/*	Complexity: O(1)											*/
srtlist_t *SrtListCreate(cmp_func_t cmp_func);

/*	Complexity: O(n)											*/
void SrtListDestroy(srtlist_t *list);

/*	Complexity: O(1)											*/
srtlist_iter_t SrtListBegin(const srtlist_t *list);

/*	Complexity: O(1)											*/
srtlist_iter_t SrtListEnd(const srtlist_t *list);

/*	Complexity: O(1)											*/
srtlist_iter_t SrtListNext(srtlist_iter_t iter);

/*	Complexity: O(1)											*/
srtlist_iter_t SrtListPrev(srtlist_iter_t iter);

/*	Complexity: O(1)											*/
int SrtListIsIterSame(srtlist_iter_t iter1, srtlist_iter_t iter2);

/*	Complexity: O(n)											*/
/*when fail return the end of list*/     
/* if there are a number of elements with the same value as the new value insert after them */

srtlist_iter_t SrtListInsert(srtlist_t *list, void* data);

/*	Complexity: O(1)                                           
 return the next element*/
srtlist_iter_t SrtListRemove(srtlist_iter_t to_remove);

/*	Complexity: O(1)											*/
void *SrtListGetData(srtlist_iter_t iter);

/*	Complexity: O(n)											*/
size_t SrtListCount(const srtlist_t *list);

/*	Complexity: O(1)											*/
int SrtListIsEmpty(const srtlist_t *list);

/*	Complexity: O(n)											*/			
srtlist_iter_t SrtListFind(srtlist_t *list, srtlist_iter_t from,
					srtlist_iter_t to,
					void *data);

/*	Complexity: O(n)											*/
srtlist_iter_t SrtListFindIf(srtlist_iter_t from,              
					srtlist_iter_t to,
					match_func_t is_match,
					void *param);

/*	Complexity: O(n)											*/
/*note : dont send callback that change the elements value because it will 
defect the sort*/   
int SrtListForEach(srtlist_iter_t from,                     
				srtlist_iter_t to,
				action_func_t action,
				void *param);

/*	Complexity: O(1)											*/
void *SrtListPopBack(srtlist_t *list);

/*	Complexity: O(1)											*/
void *SrtListPopFront(srtlist_t *list);

/*	Complexity: O(n)											*/
void SrtListMerge(srtlist_t *dest, srtlist_t *src);        

#endif /* SRTLIST_H */