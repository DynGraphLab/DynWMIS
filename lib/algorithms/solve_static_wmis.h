#ifndef MIS_SOLVE_H
#define MIS_SOLVE_H

// local includes
#include "data_structure/graph_access.h"
#include "mis_branch_and_reduce_algorithm.h"
#include "branch_and_reduce_algorithm.h"
#include "definitions.h"
#include "definitions_wmis.h"
#include "ils.h"
#include "local_search.h"
#include "online_ils.h"
#include "random_functions.h"
#include "DynWMISConfig.h"
#include "graph_io.h"
#include "mwis_graph_io.h"


// system includes
class solve_static_wmis {
        public:
                solve_static_wmis();
                virtual ~solve_static_wmis() {};

                void solve_subgraph(DynWMISConfig & config, graph_access &H, int old_mis_weight) {
                        if (config.local_algorithmType == BRANCH_AND_REDUCE) {
                                H.sort_local_adj();
                                branch_and_reduce_algorithm reducer(H, config.kamis_wmis);
                                reducer.set_lower_weight(old_mis_weight); // TODO
                                reducer.run_branch_reduce();
                                reducer.apply_branch_reduce_solution(H); 

                        } else if (config.local_algorithmType == WEIGHTED_ILS) {

                                ils local(config.kamis_wmis);
                                local.perform_ils(H, config.kamis_wmis.ils_iterations);

                        } else if (config.local_algorithmType == ONLINEMIS) {

                                onlinemis::online_ils online;
                                H.sort_local_adj();
                                online.perform_ils(config.onlinemis, H, config.onlinemis.ils_iterations, omis_by_unsucc_iterations);

                        }  else if (config.local_algorithmType == MIS_BRANCH_AND_REDUCE) {

                                // build adj arrays for mis_branch_and_reduce_algorithm
                                std::vector<std::vector<int>> adj_H;
                                adj_H.resize(H.number_of_nodes());
                                for (NodeID node = 0; node < H.number_of_nodes(); ++node) {
                                        adj_H[node].resize(H.getNodeDegree(node));
                                        EdgeID current_edge = 0;
                                        forall_out_edges (H, e, node)
                                        {
                                                adj_H[node][current_edge++] = static_cast<int>(H.getEdgeTarget(e));
                                        } endfor
                                        std::sort(adj_H[node].begin(), adj_H[node].end());
                                 }

                                mis_branch_and_reduce_algorithm reducer(adj_H, static_cast<int>(H.number_of_nodes()));

                                reducer_t.restart();
                                int res = reducer.solve(reducer_t, config.kamis_wmis.time_limit);

                                if (res != -1) {
                                        NodeWeight wis_size_H = H.number_of_nodes() - static_cast<NodeWeight>(res); // res is size of vertex cover
                                        for (NodeID node = 0; node < H.number_of_nodes(); ++node) {
                                                // for mis_branch_and_reduce a zero means
                                                if (reducer.y[node] == 0) {
                                                        H.setPartitionIndex(node, 1);
                                                } else {
                                                        H.setPartitionIndex(node, 0);
                                                }
                                        }

                                } 
                        }

                }

                void set_omis_by_unsucc_iterations() {omis_by_unsucc_iterations = true;}

       private:
                bool omis_by_unsucc_iterations; // choice stopping critertion for omis
                timer reducer_t;

};


#endif  // MIS_BRANCH_AND_REDUCE_SOLVER_H



