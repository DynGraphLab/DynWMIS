//
//

#include <cmath>
#include "dyn_wmis_simple.h"
#include "random_functions.h"

dyn_wmis_simple::dyn_wmis_simple(const std::shared_ptr<dyn_graph_access>& dyn_graph, const DynWMISConfig& config)
        : dyn_wmis_algorithm(dyn_graph, config) {
                m_is_IS.resize(dyn_graph->number_of_nodes());
                for( unsigned i = 0; i < m_is_IS.size(); i++) {
                        m_is_IS[i] = true;
                }
 }

void dyn_wmis_simple::handleInsertion(NodeID source, NodeID target) {
        G->new_edge(source, target);
        G->new_edge(target, source);

        if(m_is_IS[source] && m_is_IS[target] ) {
                if(random_functions::nextBool()) {
                        m_is_IS[source] = false;
                } else {
                        m_is_IS[target] = false;
                }
        }
}

void dyn_wmis_simple::handleDeletion(NodeID source, NodeID target) {
        G->remove_edge(source, target);
        G->remove_edge(target, source);
 }


