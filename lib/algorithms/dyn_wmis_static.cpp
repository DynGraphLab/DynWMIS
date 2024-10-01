//
//

#include <cmath>
#include "dyn_wmis_static.h"
#include "random_functions.h"

dyn_wmis_static::dyn_wmis_static(const std::shared_ptr<dyn_graph_access>& dyn_graph, const DynWMISConfig& config)
        : dyn_wmis_algorithm(dyn_graph, config), m_config(config) {
                m_is_IS.resize(dyn_graph->number_of_nodes());
                for( unsigned i = 0; i < m_is_IS.size(); i++) {
                        m_is_IS[i] = true;
                }
 }

void dyn_wmis_static::handleInsertion(NodeID source, NodeID target) {
        G->new_edge(source, target);
        G->new_edge(target, source);
}

void dyn_wmis_static::handleDeletion(NodeID source, NodeID target) {
        G->remove_edge(source, target);
        G->remove_edge(target, source);
 }


