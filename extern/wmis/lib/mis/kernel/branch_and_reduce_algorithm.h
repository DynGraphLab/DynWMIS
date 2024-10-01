/******************************************************************************
 * branch_and_reduce_algorithm.h
 *
 * Copyright (C) 2015-2017 Darren Strash <strash@kit.edu>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#ifndef BRANCH_AND_REDUCE_SOLVER_H
#define BRANCH_AND_REDUCE_SOLVER_H

// local includes
#include "data_structure/dynamic_graph.h"
#include "data_structure/graph_access.h"
#include "data_structure/sized_vector.h"
#include "definitions.h"
#include "extended_struction.h"
#include "fast_set.h"
#include "key_functions.h"
#include "mis_config.h"
#include "mwis_finder.h"
#include "original_struction.h"
#include "reductions.h"
#include "timer.h"

// system includes
#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class cout_handler {
   private:
    static std::streambuf* cout_rdbuf_backup;
    static std::stringstream buffered_output;
    static int disable_count;

    cout_handler() = delete;

   public:
    static void disable_cout() {
        disable_count++;

        if (disable_count == 1) {
            buffered_output.str(std::string());
            buffered_output.clear();
            std::cout.rdbuf(buffered_output.rdbuf());
        }
    }

    static void enable_cout() {
        if (disable_count == 0) return;

        disable_count--;

        if (disable_count == 0) std::cout.rdbuf(cout_rdbuf_backup);
    }
};

class branch_and_reduce_algorithm {
   public:
    enum IS_status { not_set, included, excluded, folded };

    size_t min_kernel;
    size_t max_min_kernel_comp = 0;

    double kernelization_time;
    double best_is_time;
    bool timeout = false;

   private:
    friend general_reduction;
    friend neighborhood_reduction;
    friend clique_neighborhood_reduction;
    friend clique_neighborhood_reduction_fast;
    friend critical_set_reduction;
    friend fold2_reduction;
    friend clique_reduction;
    friend twin_reduction;
    friend domination_reduction;
    friend generalized_neighborhood_reduction;
    friend generalized_fold_reduction;

    friend path_reduction;
    template <typename struction_type, reduction_type type, int new_nodes>
    friend class iterative_struction;
    template <typename pick_function, typename struction_type, reduction_type type>
    friend class blow_up_struction;

    friend mwis_finder;
    friend extended_struction<false>;
    friend extended_struction<true>;
    friend original_struction<false>;
    friend original_struction<true>;

    friend ApproximateIncreaseKey;
    friend DegreeKey;
    friend IncreaseKey;

    struct node_pos {
        NodeID node;
        size_t pos;

        node_pos(NodeID node = 0, size_t pos = 0) : node(node), pos(pos) {}
    };

    struct graph_status {
        size_t n = 0;
        size_t remaining_nodes = 0;
        NodeWeight is_weight = 0;
        NodeWeight reduction_offset = 0;
        dynamic_graph graph;
        std::vector<NodeWeight> weights;
        std::vector<IS_status> node_status;

        std::vector<reduction_ptr> transformations;  // reductions + blow_ups.
        size_t num_reductions;

        sized_vector<reduction_type> folded_stack;
        sized_vector<node_pos> branching_stack;
        sized_vector<NodeID> modified_stack;

        NodeWeight lower_weight = 0;
        bool use_lower_weight = false;

        graph_status() = default;

        graph_status(graph_access& G)
            : n(G.number_of_nodes()),
              remaining_nodes(n),
              graph(G),
              weights(n, 0),
              node_status(n, IS_status::not_set),
              folded_stack(n),
              branching_stack(n),
              modified_stack(n + 1) {
            forall_nodes (G, node) {
                weights[node] = G.getNodeWeight(node);
            }
            endfor
        }

        void resize(size_t size) {
            weights.resize(size, 0);
            node_status.resize(size, IS_status::not_set);
            modified_stack.resize(size + 1);
            branching_stack.resize(size);
            folded_stack.resize(size);
            n = size;
        }

        void push_nodes(size_t nodes) {
            remaining_nodes += nodes;
            graph.push_nodes(nodes);
            resize(n + nodes);
        }

        void pop_nodes(size_t nodes) {
            for (NodeID n = this->n - nodes; n < this->n; ++n) remaining_nodes -= node_status[n] == IS_status ::not_set;
            graph.pop_nodes(nodes);
            resize(n - nodes);
        }

        void decr_lower_weight(NodeWeight weight) {
            if(use_lower_weight) {
                if(lower_weight > weight) {
                    lower_weight -= weight;
                }else {
                    lower_weight = 0;
                    use_lower_weight = false;
                }
            }
        }
    };

    static constexpr NodeID BRANCHING_TOKEN = std::numeric_limits<NodeID>::max();

    static bool is_token(NodeID node) { return node == BRANCHING_TOKEN; }

    // lower graph size limit for when to use ils pruning
    static constexpr size_t ILS_SIZE_LIMIT = 50;

    // min number of remaining nodes to split up connected components
    static constexpr size_t SPLIT_CC_LIMIT = 100;

    // max number of neighbors to recurse on induced neighborhood graph in reductions
    static constexpr size_t REDU_RECURSION_LIMIT = 150;

    // max number of nodes to use all reductions during branch reduce
    static constexpr size_t FULL_REDUCTIONS_RECURSION_LIMIT = 50;

    graph_access global_graph;
    MISConfig config;
    graph_status best_solution_status;
    NodeWeight best_weight = 0;
    timer t;
    bool is_ils_best_solution = false;
    bool is_init_best_solution = false;

    bool blowing_up = false;

    // branching limit
    size_t length_non_impr_seq;

    graph_status global_status;
    std::vector<NodeID> global_mapping;
    std::vector<size_t> global_transformation_map;
    size_t total_ils_node_count;

    graph_status status;
    graph_access* local_graph;
    std::vector<NodeID> local_mapping;
    std::vector<size_t> local_transformation_map;
    std::vector<reduction_ptr> local_reductions;

    graph_access recursive_graph;
    std::vector<NodeID> recursive_mapping;
    std::vector<int> recursive_comp_map;
    std::vector<NodeID> recursive_local_mapping;

    std::function<void()> set_local_reductions;

    // presized objects for temporary use
    fast_set set_1;
    fast_set set_2;
    fast_set double_set;
    std::vector<sized_vector<NodeID>> buffers;
    std::vector<bool> bool_buffer;
    std::vector<NodeID> zero_vec;

    void resize(size_t size);

    template <bool update_dependency_checking = true>
    void set(NodeID node, IS_status status, bool push_modified = true);
    void unset(NodeID node, bool restore = true);

    void fill_global_greedy();
    void compute_ils_pruning_bound();
    NodeWeight compute_cover_pruning_bound();

    void init_transformation_step(reduction_ptr& reduction);
    void add_next_level_node(NodeID node);
    void add_next_level_neighborhood(NodeID node);
    void add_next_level_neighborhood(const std::vector<NodeID>& nodes);

    void reduce_graph_internal();
    bool blow_up_graph_internal();
    void cyclic_blow_up();
    bool branch_reduce_recursive();
    void branch_reduce_single_component();
    void initial_reduce();

    void update_best_solution();
    void update_best_solution_at_leaf();
    void undo_blow_up();
    void reverse_branching();
    void restore_best_local_solution();
    void restore_best_global_solution();

    void build_global_graph_access();
    void build_induced_neighborhood_subgraph(graph_access& G, NodeID source_node);
    void build_induced_subgraph(graph_access& G, const sized_vector<NodeID>& nodes, const fast_set& nodes_set,
                                sized_vector<NodeID>& reverse_mapping);

    void push_nodes(size_t nodes);
    void pop_nodes(size_t nodes);

   public:
    branch_and_reduce_algorithm(graph_access& G, const MISConfig& config, bool called_from_fold = false, size_t init_length_non_impr_seq = 0);

    void set_lower_weight(NodeWeight init_weight);

    graph_access& kernelize();
    size_t deg(NodeID node) const;
    void reduce_graph();
    bool run_branch_reduce();

    NodeWeight impr_by_comp_lb = 0;
    NodeWeight impr_by_branching = 0;
    bool is_init_best() const { return is_init_best_solution; }

    static size_t run_ils(const MISConfig& config, graph_access& G, sized_vector<NodeID>& tmp_buffer, size_t max_swaps);
    static void greedy_initial_is(graph_access& G, sized_vector<NodeID>& tmp_buffer);

    NodeWeight get_current_is_weight() const;
    void reverse_reduction(graph_access& G, graph_access& reduced_G, std::vector<NodeID>& reverse_mapping);
    void apply_branch_reduce_solution(graph_access& G);

    void build_graph_access(graph_access& G, std::vector<NodeID>& reverse_mapping) const;
};

#endif  // BRANCH_AND_REDUCE_SOLVER_H
