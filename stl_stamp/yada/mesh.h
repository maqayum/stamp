/*
 * PLEASE SEE LICENSE FILE FOR LICENSING AND COPYRIGHT INFORMATION
 */

#pragma once

#include <random>
#include "element.h"
#include "map.h"
#include "vector.h"
#include "list.h"
#include "queue.h"
#include "set.h"

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
};

/* =============================================================================
 * TMmesh_removeBoundary
 * =============================================================================
 */
__attribute__((transaction_safe))
bool
TMmesh_removeBoundary ( mesh_t* meshPtr, edge_t* boundaryPtr);


/* =============================================================================
 * mesh_read
 *
 * Returns number of elements read from file.
 *
 * Refer to http://www.cs.cmu.edu/~quake/triangle.html for file formats.
 * =============================================================================
 */
long
mesh_read (mesh_t* meshPtr, const char* fileNamePrefix);


/* =============================================================================
 * mesh_getBad
 * -- Returns NULL if none
 * =============================================================================
 */
element_t*
mesh_getBad (mesh_t* meshPtr);


/* =============================================================================
 * mesh_shuffleBad
 * =============================================================================
 */
void
mesh_shuffleBad (mesh_t* meshPtr, std::mt19937* randomPtr);


/* =============================================================================
 * mesh_check
 * =============================================================================
 */
bool
mesh_check (mesh_t* meshPtr, long expectedNumElement);
