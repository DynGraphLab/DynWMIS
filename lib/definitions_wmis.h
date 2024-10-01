/******************************************************************************
 * definitions.h
 *
 *****************************************************************************/

#ifndef DEFINITIONS_DYNWMIS_H
#define DEFINITIONS_DYNWMIS_H


#include <limits>
#include <queue>
#include <vector>

// use this if you need to handle negative node weights
// or you do subtractions of node weights
//typedef signed int SignedNodeWeight;
typedef unsigned int EdgeID;
typedef unsigned int NodeID;
typedef unsigned int NodeWeight;
typedef int EdgeWeight;
typedef unsigned int PartitionID;
typedef double EdgeRatingType;

struct DynNode {
    EdgeID firstEdge;
};

struct DynEdge {
    NodeID target;
};

/**
 * algorithms
 */
typedef enum { DYNWMIS_SIMPLE, DYNWMIS_GREEDY, DYNWMIS_GREEDY_DEG, DYNWMIS_BFS, DYNWMIS_STATIC } DynWMISAlgorithmType;
typedef enum { BRANCH_AND_REDUCE, WEIGHTED_ILS, ONLINEMIS, MIS_BRANCH_AND_REDUCE } DynWMISLocalAlgorithmType;
typedef enum { WEIGHTS_UNIT, WEIGHTS_RND, WEIGHTS_MOD } DynWMISWeightType;

#endif
