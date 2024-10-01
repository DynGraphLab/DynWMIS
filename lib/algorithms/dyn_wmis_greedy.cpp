//
//

#include <cmath>
#include "dyn_wmis_greedy.h"
#include "random_functions.h"

dyn_wmis_greedy::dyn_wmis_greedy(const std::shared_ptr<dyn_graph_access>& dyn_graph, const DynWMISConfig& config)
        : dyn_wmis_algorithm(dyn_graph, config) {
                m_is_IS.resize(dyn_graph->number_of_nodes());
                for( unsigned i = 0; i < m_is_IS.size(); i++) {
                        m_is_IS[i] = true;
                }
        }

void dyn_wmis_greedy::handleInsertion(NodeID source, NodeID target) {
        G->new_edge(source, target);
        G->new_edge(target, source);

        if(m_is_IS[source] && m_is_IS[target] ) {
                // keep the larger one
                if(G->getNodeWeight(source) < G->getNodeWeight(target) ) {
                        m_is_IS[source] = false;
                } else if( G->getNodeWeight(source) > G->getNodeWeight(target)) {
                        m_is_IS[target] = false;
                } else {
                        if(random_functions::nextBool()) {
                                m_is_IS[source] = false;
                        } else {
                                m_is_IS[target] = false;
                        }
                }
                if(!m_is_IS[source])  {
                        // check if any of the neighbors can go in
                         forall_out_edges((*G), e, source) {
                                 NodeID target_ = G->getEdgeTarget(source, e);
                                 bool is_free = true;
                                 forall_out_edges((*G), e_bar, target_) {
                                        NodeID target__ = G->getEdgeTarget(target_, e_bar);
                                         if(m_is_IS[target__]) {
                                                is_free = false;
                                                break;
                                         }
                                 } endfor
                                 if(is_free) {
                                         m_is_IS[target_] = true;
                                 }
                         } endfor
                }
                if(!m_is_IS[target])  {
                        // check if any of the neighbors can go in
                         forall_out_edges((*G), e, target) {
                                 NodeID target_ = G->getEdgeTarget(target, e);
                                 bool is_free = true;
                                 forall_out_edges((*G), e_bar, target_) {
                                        NodeID target__ = G->getEdgeTarget(target_, e_bar);
                                         if(m_is_IS[target__]) {
                                                is_free = false;
                                                break;
                                         }
                                 } endfor
                                 if(is_free) {
                                         m_is_IS[target_] = true;
                                 }
                         } endfor
                }
        }
}

void dyn_wmis_greedy::handleDeletion(NodeID source, NodeID target) {
        G->remove_edge(source, target);
        G->remove_edge(target, source);
}


