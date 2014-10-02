/*
 * PLEASE SEE LICENSE FILE FOR LICENSING AND COPYRIGHT INFORMATION
 */

#pragma once

#include <random>
#include <vector>
#include "element.h"
#include "queue.h"
#include "set.h"
#include "map.h"

struct mesh_t {
    element_t* rootElementPtr;
    queue_t* initBadQueuePtr;
    long size;
    SET_T* boundarySetPtr;

    mesh_t();
    ~mesh_t();

    __attribute__((transaction_safe))
    void insert(element_t* elementPtr, MAP_T* edgeMapPtr);

    __attribute__((transaction_safe))
    void remove(element_t* elementPtr);

    __attribute__((transaction_safe))
    bool insertBoundary(edge_t* boundaryPtr);

    __attribute__((transaction_safe))
    bool removeBoundary(edge_t* boundaryPtr);

    /*
     * mesh_read: Returns number of elements read from file.
     *
     * Refer to http://www.cs.cmu.edu/~quake/triangle.html for file formats.
     */
    long read(const char* fileNamePrefix);

    /*
     * mesh_getBad: Returns NULL if none
     */
    element_t* getBad();

    void shuffleBad(std::mt19937* randomPtr);

    bool check(long expectedNumElement);
};
