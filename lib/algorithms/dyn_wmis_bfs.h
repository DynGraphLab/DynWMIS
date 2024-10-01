#ifndef DYNMISBFS_H 
#define DYNMISBFS_H

#include <memory>

#include "DynWMISConfig.h"
#include "dyn_wmis_algorithm.h"
#include "dyn_graph_access.h"
#include "priority_queues/bucket_pq.h"

class dyn_wmis_bfs : public dyn_wmis_algorithm {
        public:
                dyn_wmis_bfs(const std::shared_ptr<dyn_graph_access>& dyn_graph, const DynWMISConfig & config);

                void handleInsertion(NodeID source, NodeID target) override;
                void handleDeletion(NodeID source, NodeID target) override;
                void end() {
                       forall_nodes((*G), node) {
                               if( m_is_IS[node] ) {
                                       forall_out_edges((*G), e, node) {
                                               NodeID target = G->getEdgeTarget(node, e);
                                               if( m_is_IS[target] ) {
                                                       std::cout <<  "NOT AN INDEPENDENT SET (" << node << "," << target << ")"  << std::endl;
                                                       exit(0);
                                               }
                                       } endfor
                               }
                       } endfor

                };

                virtual NodeWeight weight() {
                       NodeWeight sum = 0;
                       forall_nodes((*G), node) {
                               if(m_is_IS[node]) {
                                    sum+=G->getNodeWeight(node);
                               }
                       } endfor
                       return sum;
                }

                void doBFSnSolve(NodeID u, NodeID v);

                virtual std::vector<bool> getMIS() {return m_is_IS;};
        private:


                bool isIndependentSet(graph_access& G, const std::vector<int>& subset);
                int totalWeight(graph_access& G, const std::vector<int>& subset);
                void findSubsets(const std::vector<int>& set, std::vector<std::vector<int>>& subsets);
                void findOptimalIndependentSet(graph_access& G);
                std::vector< bool > m_is_IS;
                std::vector< int >  m_deepth;
                std::vector< int >  m_map_old_to_subgraphID;
                int update_counter;
                int max_solution_node_degree;
                NodeID last_source;
                int m_rare_update_counter;

};
#endif  
