/******************************************************************************
 * ils.h
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

#ifndef _ILS_H_
#define _ILS_H_

#include <vector>

#include "timer.h"
#include "mis_config.h"
#include "local_search.h"
#include "data_structure/graph_access.h"

class ils {
    public:
        /**
         * Default Constructor.
         */
        ils(const MISConfig& config);

        /**
         * Default Destructor.
         */
        virtual ~ils();

        /**
         * Main algorithm of the ILS.
         * More detailed information can be found in the original paper
         * of Andrade et al.
         *
         * @param config Config used for the ILS.
         * @param G Graph representation.
         * @param iteration_limit Maximum number of iterations.
         */
        void perform_ils(graph_access& G, unsigned int iteration_limit = 0);

        /** 
         * Reset the ILS.
         * Clears the force-list and best solution.
         */
        void reset();

    private:
		// Config for ils
		MISConfig config;
        // Array for storing the last time a node was forced in the solution.
        std::vector<NodeID> last_forced;
        // Main solution data structure.
        mis_permutation *perm;
        // List of candidates for the local search.
		maxNodeHeap *cand;
        // List of nodes that were forced in the solution.
        candidate_list *force_list;
        // List of onetight nodes.
        candidate_list *one;
        // Best solution found so far
        NodeID *best_solution;
        NodeID best_solution_size;
		NodeWeight best_solution_weight = 0;
        // Local search algorithm
        local_search local;
        // Timer for measuring the time taken for the ILS.
        timer t;

        // ILS config
        unsigned int plateau_down;
        unsigned int plateau_up;
        unsigned int plateau_best;
        unsigned int plateau_best_again;
        unsigned int pden_floor;
        unsigned int delta_penalty;
        bool limit_plateau;
        bool swap_on_failure;

        /**
         * Force the given node in the solution.
         *
         * @param config Config used for the ILS.
         * @param G Graph representation.
         * @param v Node to force into the solution.
         * @param force_list List storing the forced nodes.
         */
        void force(MISConfig& config, graph_access& G, NodeID v, candidate_list *force_list = NULL);

        /**
         * Undo all operations currently stored in the operation log.
         *
         * @param G Graph representation
         */
        void unwind(graph_access& G);
};

#endif
