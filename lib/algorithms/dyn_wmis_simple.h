#ifndef DELTA_ORIENTATIONS_BFSCS_H
#define DELTA_ORIENTATIONS_BFSCS_H

#include <memory>

#include "DynWMISConfig.h"
#include "dyn_wmis_algorithm.h"
#include "dyn_graph_access.h"
#include "priority_queues/bucket_pq.h"

class dyn_wmis_simple : public dyn_wmis_algorithm {
        public:
                dyn_wmis_simple(const std::shared_ptr<dyn_graph_access>& dyn_graph, const DynWMISConfig & config);

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


                virtual std::vector<bool> getMIS() {return m_is_IS;};
        private:

                std::vector< bool > m_is_IS;

};



#endif  
