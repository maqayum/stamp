/* =============================================================================
 *
 * heap.c
 *
 * =============================================================================
 *
 * Copyright (C) Stanford University, 2006.  All Rights Reserved.
 * Author: Chi Cao Minh
 *
 * =============================================================================
 *
 * For the license of bayes/sort.h and bayes/sort.c, please see the header
 * of the files.
 *
 * ------------------------------------------------------------------------
 *
 * For the license of kmeans, please see kmeans/LICENSE.kmeans
 *
 * ------------------------------------------------------------------------
 *
 * For the license of ssca2, please see ssca2/COPYRIGHT
 *
 * ------------------------------------------------------------------------
 *
 * For the license of lib/mt19937ar.c and lib/mt19937ar.h, please see the
 * header of the files.
 *
 * ------------------------------------------------------------------------
 *
 * For the license of lib/rbtree.h and lib/rbtree.c, please see
 * lib/LEGALNOTICE.rbtree and lib/LICENSE.rbtree
 *
 * ------------------------------------------------------------------------
 *
 * Unless otherwise noted, the following license applies to STAMP files:
 *
 * Copyright (c) 2007, Stanford University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of Stanford University nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY STANFORD UNIVERSITY ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL STANFORD UNIVERSITY BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * =============================================================================
 */


#include <stdlib.h>
#include <assert.h>
#include "heap.h"
#include "tm.h"

struct heap {
    void** elements;
    long size;
    long capacity;
    // [wer] mark the comparator TM_SAFE
    TM_SAFE long (*compare)(const void*, const void*);
};


#define PARENT(i)       ((i) / 2)
#define LEFT_CHILD(i)   (2*i)
#define RIGHT_CHILD(i)  (2*(i) + 1)

/* =============================================================================
 * DECLARATION OF TM_SAFE FUNCTIONS
 * =============================================================================
 */

TM_SAFE
void
siftUp (heap_t* heapPtr, long startIndex);

TM_SAFE
void
TMheapify (heap_t* heapPtr, long startIndex);


/* =============================================================================
 * heap_alloc
 * -- Returns NULL on failure
 * =============================================================================
 */
heap_t*
heap_alloc (long initCapacity, long (*compare)(const void*, const void*))
{
    heap_t* heapPtr;

    heapPtr = (heap_t*)malloc(sizeof(heap_t));
    if (heapPtr) {
        long capacity = ((initCapacity > 0) ? (initCapacity) : (1));
        heapPtr->elements = (void**)malloc(capacity * sizeof(void*));
        assert(heapPtr->elements);
        heapPtr->size = 0;
        heapPtr->capacity = capacity;
        heapPtr->compare = compare;
    }

    return heapPtr;
}


/* =============================================================================
 * heap_free
 * =============================================================================
 */
void
heap_free (heap_t* heapPtr)
{
    free(heapPtr->elements);
    free(heapPtr);
}


/* =============================================================================
 * siftUp
 * =============================================================================
 */
TM_SAFE
void siftUp (heap_t* heapPtr, long startIndex)
{
    void** elements = heapPtr->elements;
    long TM_SAFE (*compare)(const void*, const void*) = heapPtr->compare;

    long index = startIndex;
    while ((index > 1)) {
        long parentIndex = PARENT(index);
        void* parentPtr = elements[parentIndex];
        void* thisPtr   = elements[index];
        if (compare(parentPtr, thisPtr) >= 0) {
            break;
        }
        void* tmpPtr = parentPtr;
        elements[parentIndex] = thisPtr;
        elements[index] = tmpPtr;
        index = parentIndex;
    }
}


/* =============================================================================
 * heap_insert
 * -- Returns false on failure
 * =============================================================================
 */
TM_SAFE
bool
heap_insert (heap_t* heapPtr, void* dataPtr)
{
    long size = heapPtr->size;
    long capacity = heapPtr->capacity;

    if ((size + 1) >= capacity) {
        long newCapacity = capacity * 2;
        void** newElements = (void**)malloc(newCapacity * sizeof(void*));
        if (newElements == NULL) {
            return false;
        }
        heapPtr->capacity = newCapacity;
        long i;
        void** elements = heapPtr->elements;
        for (i = 0; i <= size; i++) {
            newElements[i] = elements[i];
        }
        free(heapPtr->elements);
        heapPtr->elements = newElements;
    }
    size ++;
    heapPtr->size = size;
    //size = ++(heapPtr->size);
    heapPtr->elements[size] = dataPtr;
    siftUp(heapPtr, size);

    return true;
}


/* =============================================================================
 * TMheap_insert
 * -- Returns false on failure
 * =============================================================================
 */
#if 0
TM_SAFE
bool
TMheap_insert (  heap_t* heapPtr, void* dataPtr)
{
    long size = (long)TM_SHARED_READ(heapPtr->size);
    long capacity = (long)TM_SHARED_READ(heapPtr->capacity);

    if ((size + 1) >= capacity) {
        long newCapacity = capacity * 2;
        void** newElements = (void**)TM_MALLOC(newCapacity * sizeof(void*));
        if (newElements == NULL) {
            return false;
        }
        TM_SHARED_WRITE(heapPtr->capacity, newCapacity);
        long i;
        void** elements = (void **)TM_SHARED_READ_P(heapPtr->elements);
        for (i = 0; i <= size; i++) {
            newElements[i] = (void*)TM_SHARED_READ_P(elements[i]);
        }
        free(heapPtr->elements);
        TM_SHARED_WRITE_P(heapPtr->elements, newElements);
    }

    size++;
    TM_SHARED_WRITE(heapPtr->size, size);
    void** elements = (void**)TM_SHARED_READ_P(heapPtr->elements);
    TM_SHARED_WRITE_P(elements[size], dataPtr);
    siftUp(heapPtr, size);

    return true;
}
#endif

/* =============================================================================
 * heapify
 * =============================================================================
 */
#if 0
static void
heapify (heap_t* heapPtr, long startIndex)
{
    void** elements = heapPtr->elements;
    long (*compare)(const void*, const void*) = heapPtr->compare;

    long size = heapPtr->size;
    long index = startIndex;

    while (1) {

        long leftIndex = LEFT_CHILD(index);
        long rightIndex = RIGHT_CHILD(index);
        long maxIndex = -1;

        if ((leftIndex <= size) &&
            (compare(elements[leftIndex], elements[index]) > 0))
        {
            maxIndex = leftIndex;
        } else {
            maxIndex = index;
        }

        if ((rightIndex <= size) &&
            (compare(elements[rightIndex], elements[maxIndex]) > 0))
        {
            maxIndex = rightIndex;
        }

        if (maxIndex == index) {
            break;
        } else {
            void* tmpPtr = elements[index];
            elements[index] = elements[maxIndex];
            elements[maxIndex] = tmpPtr;
            index = maxIndex;
        }
    }
}
#endif

/* =============================================================================
 * TMheapify
 * =============================================================================
 */
TM_SAFE
void
TMheapify (  heap_t* heapPtr, long startIndex)
{
    void** elements = (void**)TM_SHARED_READ_P(heapPtr->elements);
    //long (*compare)(const void*, const void*) TM_IFUNC_DECL = heapPtr->compare;
    long (*compare)(const void*, const void*) TM_SAFE = heapPtr->compare;

    long size = (long)TM_SHARED_READ(heapPtr->size);
    long index = startIndex;

    while (1) {
        long leftIndex = LEFT_CHILD(index);
        long rightIndex = RIGHT_CHILD(index);
        long maxIndex = -1;

        if (leftIndex <= size)
        {
            long ret;
            void *e1, *e2;

            e1 = (void*)TM_SHARED_READ_P(elements[leftIndex]);
            e2 = (void*)TM_SHARED_READ_P(elements[index]);
            TM_IFUNC_CALL2(ret, compare, e1, e2);

            if (ret > 0)
                maxIndex = leftIndex;
            else
                maxIndex = index;
        } else {
            maxIndex = index;
        }

        if (rightIndex <= size)
        {
            long ret;
            void *e1, *e2;

            e1 = (void*)TM_SHARED_READ_P(elements[rightIndex]);
            e2 = (void*)TM_SHARED_READ_P(elements[maxIndex]);
            TM_IFUNC_CALL2(ret, compare, e1, e2);

            if (ret > 0)
                maxIndex = rightIndex;
        }

        if (maxIndex == index) {
            break;
        } else {
            void* tmpPtr = (void*)TM_SHARED_READ_P(elements[index]);
            TM_SHARED_WRITE_P(elements[index],
                              (void*)TM_SHARED_READ_P(elements[maxIndex]));
            TM_SHARED_WRITE_P(elements[maxIndex], tmpPtr);
            index = maxIndex;
        }
    }
}

/* =============================================================================
 * heap_remove
 * -- Returns NULL if empty
 * =============================================================================
 */
TM_SAFE
void*
heap_remove (heap_t* heapPtr)
{
    long size = heapPtr->size;

    if (size < 1) {
        return NULL;
    }

    void** elements = heapPtr->elements;
    void* dataPtr = elements[1];
    elements[1] = elements[size];
    heapPtr->size = size - 1;
    TMheapify(heapPtr, 1);

    return dataPtr;
}

/* =============================================================================
 * heap_isValid
 * =============================================================================
 */
bool
heap_isValid (heap_t* heapPtr)
{
    long size = heapPtr->size;
    long (*compare)(const void*, const void*) = heapPtr->compare;
    void** elements = heapPtr->elements;

    long i;
    for (i = 1; i < size; i++) {
        if (compare(elements[i+1], elements[PARENT(i+1)]) > 0) {
            return false;
        }
    }

    return true;
}


/* =============================================================================
 * TEST_HEAP
 * =============================================================================
 */
#ifdef TEST_HEAP


#include <assert.h>
#include <stdio.h>


static long
compare (const void* a, const void* b)
{
    return (*((const long*)a) - *((const long*)b));
}


static void
printHeap (heap_t* heapPtr)
{
    printf("[");

    long i;
    for (i = 0; i < heapPtr->size; i++) {
        printf("%li ", *(long*)heapPtr->elements[i+1]);
    }

    puts("]");
}


static void
insertInt (heap_t* heapPtr, long* data)
{
    printf("Inserting: %li\n", *data);
    assert(heap_insert(heapPtr, (void*)data));
    printHeap(heapPtr);
    assert(heap_isValid(heapPtr));
}


static void
removeInt (heap_t* heapPtr)
{
    long* data = heap_remove(heapPtr);
    printf("Removing: %li\n", *data);
    printHeap(heapPtr);
    assert(heap_isValid(heapPtr));
}



long global_data[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9};
long global_numData = sizeof(global_data) / sizeof(global_data[0]);

int
main ()
{
    puts("Starting...");

    heap_t* heapPtr = heap_alloc(1, compare);

    assert(heapPtr);

    long i;
    for (i = 0; i < global_numData; i++) {
        insertInt(heapPtr, &global_data[i]);
    }

    for (i = 0; i < global_numData; i++) {
        removeInt(heapPtr);
    }

    assert(heap_remove(heapPtr) == NULL); /* empty */

    heap_free(heapPtr);

    puts("Passed all tests.");

    return 0;
}


#endif /* TEST_HEAP */


/* =============================================================================
 *
 * End of heap.c
 *
 * =============================================================================
 */