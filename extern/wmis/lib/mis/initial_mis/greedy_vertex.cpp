/******************************************************************************
 * greedy_vertex.cpp
 *
 * Copyright (C) 2015-2017 Sebastian Lamm <lamm@ira.uka.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *****************************************************************************/

#include "greedy_vertex.h"

#include "random_functions.h"
#include "data_structure/priority_queues/bucket_array.h"

greedy_vertex::greedy_vertex() {

}

greedy_vertex::~greedy_vertex() {

}

void greedy_vertex::initial_partition(const unsigned int seed, graph_access & G) {
    random_functions::setSeed(seed);
    NodePermutationMap permutation;
    generate_permutation(G, permutation);

    bucket_array *buckets = new bucket_array(G.number_of_nodes());
    int *uncovered_edges = new int[G.number_of_nodes()];
    G.set_partition_count(2);

    // Initialize the priority queue
    forall_nodes (G, n) {
        NodeID node = permutation[n];
        buckets->increment(node, G.getMaxDegree() - G.getNodeDegree(node));
        uncovered_edges[node] = G.getNodeDegree(node);
        G.setPartitionIndex(node, 0);
    } endfor

    // While there are still vertices left
    while (1) {
        // Pick the node with the most uncovered edges
        int start_node = buckets->pickSmallest();
        if (start_node == -1) break;
        buckets->remove(start_node);
        uncovered_edges[start_node] = 0;
        // Insert it into the solution
        G.setPartitionIndex(start_node, 1);
        
        forall_out_edges(G, edge, start_node) {
            NodeID target = G.getEdgeTarget(edge);
            // Remove any neighbors that are now completely covered
            if (buckets->contains(target)) {
                buckets->increment(target);
                uncovered_edges[target]--;
                if (uncovered_edges[target] <= 0) {
                    buckets->remove(target);
                }
            }
        } endfor
    }

    // Build complement
    forall_nodes(G, node) {
        if (G.getPartitionIndex(node) == 0) G.setPartitionIndex(node, 1); 
        else G.setPartitionIndex(node, 0);
    } endfor

    delete buckets;
}

void greedy_vertex::generate_permutation(graph_access & G, NodePermutationMap & permutation) {
    permutation.resize(G.number_of_nodes());
    random_functions::permutate_vector_good(permutation, true);
}

