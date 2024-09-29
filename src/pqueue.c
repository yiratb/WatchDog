/****************************
Yirat Romano                *
29/02/2024                  *
Mia                         *
source file                 *
exercise: pqueue            *
*****************************/
#include <stdlib.h> /*malloc*/
#include <assert.h>

#include "srtlist.h"

typedef struct pq
{
    srtlist_t *srtlist;
} pq_t;   

pq_t *PQCreate(cmp_func_t cmp_func)
{
    pq_t *pq = {NULL};

    assert(cmp_func);

    pq = (pq_t*)malloc(sizeof(pq_t));
    if (NULL == pq)
    {
        return NULL;
    }

    pq->srtlist = SrtListCreate(cmp_func);
    if (NULL == pq->srtlist)
    {
        free(pq);
        return NULL;
    }

    return (pq);
}

void PQDestroy(pq_t *pq)
{
    assert(pq);

    SrtListDestroy(pq->srtlist);
    pq->srtlist = NULL;

    free(pq);
}

int PQEnqueue(pq_t *pq, void *data)
{
    assert(pq);

    return (SrtListIsIterSame(SrtListInsert(pq->srtlist, data), SrtListEnd(pq->srtlist)));
}

void *PQDequeue(pq_t *pq)
{
    assert(pq);

    return (SrtListPopFront(pq->srtlist));
}

void *PQPeek(const pq_t *pq)
{
    assert(pq);

    return (SrtListGetData(SrtListBegin(pq->srtlist)));
}

int PQIsEmpty(const pq_t *pq)
{
    assert(pq);

    return (SrtListIsEmpty(pq->srtlist));
}

size_t PQCount(const pq_t *pq)
{
    assert(pq);

    return (SrtListCount(pq->srtlist));
}

void *PQErase(pq_t *pq, match_func_t match_func, void *param)
{
    void *data = NULL;
    srtlist_iter_t iter;
    assert(pq);
    assert(match_func);

    iter = SrtListFindIf(SrtListBegin(pq->srtlist), SrtListEnd(pq->srtlist), 
                                                                match_func, param);
    if (SrtListIsIterSame(iter, SrtListEnd(pq->srtlist)))
    {
        return NULL;
    }
    data = SrtListGetData(iter);
    SrtListRemove(iter);

    return (data);
}

void PQClear(pq_t *pq)
{
    assert(pq);

    while (!PQIsEmpty(pq))
    {
        PQDequeue(pq);
    }
}
