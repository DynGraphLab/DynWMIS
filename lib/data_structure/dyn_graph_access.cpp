/******************************************************************************
 * dyn_graph_access.cpp
 *
 *
 *****************************************************************************/

#include "dyn_graph_access.h"

dyn_graph_access::dyn_graph_access() : m_building_graph(false) {
        edge_count = 0;
        node_count = 0;
}

dyn_graph_access::dyn_graph_access(NodeID n) : m_building_graph(false) {
        edge_count = 0;
        node_count = n;
        start_construction(node_count, edge_count);
        finish_construction();
}

dyn_graph_access::~dyn_graph_access() = default;

void dyn_graph_access::start_construction(NodeID nodes, EdgeID edges) {
        m_building_graph = true;
        m_edges.resize(nodes);
        m_nodes.resize(nodes);
        m_node_weight.resize(nodes);
}

NodeID dyn_graph_access::new_node() {
        ASSERT_TRUE(m_building_graph);
        return node_count++;
}

void dyn_graph_access::finish_construction() {
        m_building_graph = false;
        m_edges.resize(node_count);
}

void dyn_graph_access::convert_from_graph_access(graph_access& H) {
        std::cout <<  "TODO"  << std::endl;
        exit(0);
}

void dyn_graph_access::convert_to_graph_access(graph_access& H) {
        H.start_construction(number_of_nodes(), 2*number_of_edges());
        forall_nodes((*this), node) {
                H.new_node();
                H.setNodeWeight(node, getNodeWeight(node));

                forall_out_edges((*this), e, node) {
                        NodeID target = (*this).getEdgeTarget(node, e);
                        EdgeID e_bar = H.new_edge(node, target);
                        H.setEdgeWeight(e_bar, 1);
                } endfor
        } endfor
        
        H.finish_construction();
}







