/****************************
Yirat Romano                *
12/02/2024                  *
                       *
source file                 *
exercise: slist             *
*****************************/
#include <stdlib.h> /*malloc()*/
#include <assert.h> /*assert*/

#include "../inc/dlist.h" /*dlist_iter_t dlist_t*/

enum
{
    FAIL = -1,
    PASS,
    NOT_FOUND
};

typedef struct dlist_node node_t;

struct dlist_node 
{
    void *data;
    node_t *next;
    node_t *prev;
};

struct dlist
{
    node_t head;
    node_t tail;
};

static node_t *CreateNode(void *data, node_t *next, node_t *prev);
static node_t* IterToNode(dlist_iter_t iter);
static dlist_iter_t NodeToIter(const node_t *node);
static dlist_iter_t EndOfTheList(node_t *node);
static int IsNotInRange(dlist_iter_t from, dlist_iter_t to, dlist_iter_t who);
static int CountList(void *data, void *param);

dlist_t *DListCreate()
{
    dlist_t *list = (dlist_t*)malloc(sizeof(dlist_t));
    if (list == NULL)
    {
        return NULL;
    }

    list->tail.data = NULL;
    list->tail.next = NULL;
    list->tail.prev = &list->head;

    list->head.data = NULL;
    list->head.next = &list->tail;
    list->head.prev = NULL;

    return (list);
}

void DListDestroy(dlist_t *list)
{
    dlist_iter_t iter1, iter2;

	assert(list);
	
	iter1 = DListBegin(list);
    iter2 = iter1;
	
	while (!(DListIsIterSame(NodeToIter(iter1),DListEnd(list)))) 
	{
		iter2 = DListNext(iter2);
		free(iter1);
		iter1 = iter2;
	}
	
	free(list);
}

dlist_iter_t DListBegin(const dlist_t *list)
{
    assert (list);

    return (NodeToIter(list->head.next));
}

dlist_iter_t DListEnd(const dlist_t *list)
{
    dlist_iter_t iter = NodeToIter(&list->tail);

    assert(list);

    return (iter);
}

dlist_iter_t DListNext(dlist_iter_t iter)
{
    assert (IterToNode(iter));

    return (NodeToIter(IterToNode(iter)->next));
}

dlist_iter_t DListPrev(dlist_iter_t iter)
{
    assert (IterToNode(iter));

    return (NodeToIter(IterToNode(iter)->prev));
}


int DListIsIterSame(dlist_iter_t iter1, dlist_iter_t iter2)
{
    
    assert (IterToNode(iter2));

    return (IterToNode(iter1) == IterToNode(iter2));
}

dlist_iter_t DListInsert(dlist_iter_t where, void* data)
{
    node_t *where_node = IterToNode(where), *prev_node = NULL, *new_node= NULL;

    assert (where_node);

    prev_node = where_node->prev;
    new_node = CreateNode(data, where_node, prev_node);
    if (NULL == new_node)
    {
        return EndOfTheList(where_node);
    }

    prev_node->next = new_node;
    where_node->prev = new_node;

    return NodeToIter(new_node);
}

dlist_iter_t DListRemove(dlist_iter_t to_remove)
{
    node_t *to_remove_node= IterToNode(to_remove);
    node_t *prev_node = to_remove_node->prev;
    node_t *next_node = to_remove_node->next;

    prev_node->next = next_node;
    next_node->prev = prev_node;

    free(to_remove_node);
    return(next_node);
}

void *DListGetData(dlist_iter_t iter)
{
    assert (IterToNode(iter));

    return (IterToNode(iter)->data);
}

void DListSetData(dlist_iter_t iter, void *data)
{
    assert (IterToNode(iter));

    IterToNode(iter)->data = data;
}

size_t DListCount(const dlist_t *list)
{
    size_t count = 0;

    assert (list);

    DListForEach(DListBegin(list), DListEnd(list), &CountList, &count);

    return (count);
}

int DListIsEmpty(const dlist_t *list)
{
    assert(list);

    return (DListIsIterSame(DListBegin(list), DListEnd(list)));
}

dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, match_func_t is_match, void *param)
{
    assert (IterToNode(from));
    assert (IterToNode(to));
    assert (is_match);

    while (!DListIsIterSame(from, to) && !is_match(DListGetData(from), param))
    {
        from = DListNext(from);
    }
    
    return from;
}

int DListForEach(dlist_iter_t from, dlist_iter_t to, action_func_t action, void *param)
{
    int status = PASS;

    assert (IterToNode(from));
    assert (IterToNode(to));
    assert (action);

    while (!DListIsIterSame(from, to) && PASS == status)
    {
        status = action(DListGetData(from), param);

        from = DListNext(from);
    }

    return (status);
}

void DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where)
{
    node_t *from_node = IterToNode(from), *to_node = IterToNode(to);
    node_t *where_node = IterToNode(where);
    node_t *prev_to = NULL, *prev_where = NULL;

    assert(from_node);
    assert(to_node);
    assert(where_node);
    assert(IsNotInRange(from, to, where));

    prev_to = to_node->prev;
    prev_where = where_node->prev;

    from_node->prev->next = to_node;
    to_node->prev = from_node->prev;

    prev_where->next = from;
    from_node->prev = prev_where;

    where_node->prev = prev_to;
    prev_to->next = where_node;
}

int DListMultiFind(dlist_iter_t from, dlist_iter_t to, match_func_t is_match, void *param, dlist_t *found_elements)
{
    node_t *new_node = NULL;
    int status = NOT_FOUND;

    assert (IterToNode(from));
    assert (IterToNode(to));
    assert (is_match);
    assert (found_elements);

    while (!DListIsIterSame(from, to))
    {
        if (is_match(DListGetData(from), param))
        {
            status = PASS;
            new_node = IterToNode(DListPushBack(found_elements, DListGetData(from)));
            if (DListEnd(found_elements) == NodeToIter(new_node))
            {
                return (FAIL);
            }
        }
        from = DListNext(from);
    }
    
    return status;
}

dlist_iter_t DListPushBack(dlist_t *list, void *data)
{
    assert (list);

    return DListInsert(DListEnd(list), data);
}

void *DListPopBack(dlist_t *list)
{
    void *data = NULL;

    assert(list);

    data = DListGetData(DListEnd(list)->prev);
    DListRemove(DListEnd(list)->prev);

    return data;
}

dlist_iter_t DListPushFront(dlist_t *list, void *data)
{
    assert (list);

    return DListInsert(DListBegin(list), data);
}

void *DListPopFront(dlist_t *list)
{
    void *data = NULL;
    
    assert(list);

    data = DListGetData(DListBegin(list));
    DListRemove(DListBegin(list));

    return data;
}

/**************STATIC FUNCTION*************/
static node_t *CreateNode(void *data, node_t *next, node_t *prev)
{
    node_t *new_node = NULL;

    new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        return NULL;
    }

    new_node->data = data;
    new_node->next = next;
    new_node->prev = prev;

    return (new_node);
}

static node_t* IterToNode(dlist_iter_t iter)
{
    return ((node_t*)iter);
}

static dlist_iter_t NodeToIter(const node_t *node)
{
    return ((dlist_iter_t)node);
}

static dlist_iter_t EndOfTheList(node_t *node)
{
    assert (node);

    while (NULL != node->next)
    {
        node = node->next;
    }

    return (NodeToIter(node));
}

static int IsNotInRange(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where)
{		
	while (!DListIsIterSame(from, to) && !DListIsIterSame(from, where))
	{	 
		from = DListNext(from);
	}
	
	return (!DListIsIterSame(from, where));
}

static int CountList(void *data, void *param)
{
    (void)data;

    (*(size_t *)param)++;
    return (PASS);
}
