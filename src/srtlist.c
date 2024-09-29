/****************************
Yirat Romano                *
20/02/2024                  *
                            *
source file                 *
exercise: slist             *
*****************************/
#include <stdlib.h> /*malloc()*/
#include <assert.h> /*assert*/

#include "srtlist.h" /*srtlist_t srtlist_iter_t*/
#include "dlist.h" /*dlist_t dlist_iter_t*/

struct srtlist
{
    dlist_t *dlist;
    cmp_func_t cmp_func;
};

static srtlist_iter_t DListIterToSrtListIter(srtlist_t *list, dlist_iter_t iter);
static dlist_iter_t SrtListIterToDListIter(srtlist_iter_t iter);
static srtlist_iter_t FuncMove(srtlist_t *list, srtlist_iter_t iter, void *data);

srtlist_t *SrtListCreate(cmp_func_t cmp_func)
{
    srtlist_t *list = NULL;

    assert(cmp_func);
    
    list = (srtlist_t*)malloc(sizeof(srtlist_t));
    if (NULL == list)
    {
        return NULL;
    }

    list->dlist = DListCreate();
    if (NULL == list->dlist)
    {
        free(list);
        return NULL;
    }
    list->cmp_func = cmp_func;

    return list;
}

void SrtListDestroy(srtlist_t *list)
{
    assert(list);

    DListDestroy(list->dlist);
    list->dlist = NULL;

    free(list);
}

srtlist_iter_t SrtListBegin(const srtlist_t *list)
{
    assert (list);

    return (DListIterToSrtListIter((srtlist_t *)list, DListBegin(list->dlist)));
}

srtlist_iter_t SrtListEnd(const srtlist_t *list)
{
    assert (list);

    return (DListIterToSrtListIter((srtlist_t *)list, DListEnd(list->dlist)));
}

srtlist_iter_t SrtListNext(srtlist_iter_t iter)
{
    iter.iter = DListNext(SrtListIterToDListIter(iter));

    return iter;
}

srtlist_iter_t SrtListPrev(srtlist_iter_t iter)
{
    iter.iter = DListPrev(SrtListIterToDListIter(iter));

    return iter;
}

int SrtListIsIterSame(srtlist_iter_t iter1, srtlist_iter_t iter2)
{
    return (DListIsIterSame(SrtListIterToDListIter(iter1), SrtListIterToDListIter(iter2)));
}

srtlist_iter_t SrtListInsert(srtlist_t *list, void* data)
{
    srtlist_iter_t runner = {NULL};

    assert(list);

    runner = SrtListBegin(list);

    while (!SrtListIsIterSame(runner, SrtListEnd(list)) && 
                (list->cmp_func(SrtListGetData(runner), data)) <= 0)
    {
        runner = SrtListNext(runner);
    }
    
    return (DListIterToSrtListIter(list, DListInsert(SrtListIterToDListIter(runner), data)));
} 

srtlist_iter_t SrtListRemove(srtlist_iter_t to_remove)
{
    srtlist_iter_t srt_iter = to_remove;

    srt_iter.iter = DListRemove(SrtListIterToDListIter(to_remove));

    return srt_iter;
}

void *SrtListGetData(srtlist_iter_t iter)
{
    return (DListGetData(SrtListIterToDListIter(iter)));
}

size_t SrtListCount(const srtlist_t *list)
{
    assert(list);

    return (DListCount(list->dlist));
}

int SrtListIsEmpty(const srtlist_t *list)
{
    assert(list);
    
    return (DListIsEmpty(list->dlist));
}

srtlist_iter_t SrtListFind(srtlist_t *list, srtlist_iter_t from, srtlist_iter_t to, void *data)
{
    assert(list);
    assert (from.list == from.list);

    while (!SrtListIsIterSame(from, to) && list->cmp_func(SrtListGetData(from), data) < 0)
    {
        from = SrtListNext(from);
    }
    
    return from;
}

srtlist_iter_t SrtListFindIf(srtlist_iter_t from, srtlist_iter_t to, match_func_t is_match, void *param)
{
    assert (is_match);
    assert (from.list == from.list);

    from.iter = DListFind(SrtListIterToDListIter(from), SrtListIterToDListIter(to), is_match, param);

    return from;
}
 
int SrtListForEach(srtlist_iter_t from, srtlist_iter_t to, action_func_t action, void *param)
{
    assert(action);
    assert (from.list == from.list);
    
    return(DListForEach(SrtListIterToDListIter(from), SrtListIterToDListIter(to), action, param));
}

void *SrtListPopBack(srtlist_t *list)
{
    return DListPopBack(list->dlist);
}

void *SrtListPopFront(srtlist_t *list)
{
    return DListPopFront(list->dlist);

}

void SrtListMerge(srtlist_t *dest, srtlist_t *src)
{
    srtlist_iter_t where = {NULL}, to = {NULL}, from = {NULL};

    assert (dest);
    assert (src);

    where = SrtListBegin(dest);
    to = SrtListBegin(src);
    from = to;

    while (!SrtListIsIterSame(where, SrtListEnd(dest)) &&
        !SrtListIsIterSame(to, SrtListEnd(src)))
    {
        where = FuncMove(dest, where, SrtListGetData(to));
        if (SrtListIsIterSame(where,SrtListEnd(dest)))
        {
            break;
        } 
        from = to;
        to = SrtListFind(src, to, SrtListEnd(dest), SrtListGetData(where));

        DListSplice(SrtListIterToDListIter(from), SrtListIterToDListIter(to), SrtListIterToDListIter(where));
    }

    if (!SrtListIsIterSame(to, SrtListEnd(src)))
    {
        DListSplice(SrtListIterToDListIter(to), SrtListIterToDListIter(SrtListEnd(src)), SrtListIterToDListIter(where));
    }
}

/*********************STATIC FUNCTION********************/

static srtlist_iter_t DListIterToSrtListIter(srtlist_t *list, dlist_iter_t iter)
{
    srtlist_iter_t srt_iter = {NULL};

    #ifndef NDEBUG 
    srt_iter.list = list;
    #endif 

    (void)list;

    srt_iter.iter = iter;

    return srt_iter;
}

static dlist_iter_t SrtListIterToDListIter(srtlist_iter_t iter)
{
    return iter.iter;
}

static srtlist_iter_t FuncMove(srtlist_t *list, srtlist_iter_t iter, void *data)
{
    assert (list);

    while (!SrtListIsIterSame(iter, SrtListEnd(list)) &&
            list->cmp_func(SrtListGetData(iter), data) <= 0)
    {
        iter = SrtListNext(iter);
    }

    return (iter);
}
