/******************************************************************************
 * dyn_graph_access.h
 *
 *****************************************************************************/

#ifndef dyn_graph_access_EFRXO4X2
#define dyn_graph_access_EFRXO4X2

#include "graph_access.h"

#include <bitset>
#include <cassert>
#include <iostream>
#include <sparsehash/dense_hash_map>
#include <vector>

#include "../definitions_wmis.h"
#include "macros_assertions.h"
//#include "IGraph.h"

class dyn_graph_access {
   public:
    dyn_graph_access();
    explicit dyn_graph_access(NodeID n);
    ~dyn_graph_access();

    // construction of the graph
    void start_construction(NodeID nodes, EdgeID edges);
    NodeID new_node();
    void finish_construction();

    void convert_from_graph_access(graph_access& H);
    void convert_to_graph_access(graph_access& H);

    inline bool new_edge(NodeID source, NodeID target);
    inline bool remove_edge(NodeID source, NodeID target);

    //inline bool isEdge(NodeID source, NodeID target);

    // access
    inline EdgeID number_of_edges();
    inline NodeID number_of_nodes();

    inline EdgeID get_first_edge(NodeID node);
    inline EdgeID get_first_invalid_edge(NodeID node);

    inline EdgeID getNodeDegree(NodeID node);
    inline NodeWeight getNodeWeight(NodeID node) {return m_node_weight[node];};
    inline void setNodeWeight(NodeID node, NodeWeight weight) {m_node_weight[node] = weight;};
    inline void resizeNodeWeights(int n) {m_node_weight.resize(n);};
    inline NodeID getEdgeTarget(NodeID source, EdgeID edge);

    EdgeID maxDegree() {
        EdgeID currentMax = 0;
        for (NodeID node = 0; node < node_count; ++node) {
                EdgeID deg = getNodeDegree(node);
                if (deg > currentMax) currentMax = deg;
        }
        return currentMax;
    }

   private:
    // %%%%%%%%%%%%%%%%%%% DATA %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    std::vector<std::vector<DynEdge> > m_edges;
    std::vector<NodeWeight> m_node_weight;

    std::vector<DynNode> m_nodes;

    EdgeID edge_count;
    NodeID node_count;

    // construction properties
    bool m_building_graph;
};

bool dyn_graph_access::new_edge(NodeID source, NodeID target) {
        DynEdge e_buf;
        e_buf.target = target;
        m_edges[source].push_back(e_buf);

        edge_count++;
        return true;
}

bool dyn_graph_access::remove_edge(NodeID source, NodeID target) {
        for( unsigned i = 0; i < m_edges[source].size(); i++) {
                if(m_edges[source][i].target == target) {
                        m_edges[source][i] = m_edges[source].back();;
                        m_edges[source].pop_back();
                        edge_count--;
                        return true;
                }
        }
        return false;
}


EdgeID dyn_graph_access::number_of_edges() { return edge_count; }

NodeID dyn_graph_access::number_of_nodes() { return node_count; }

EdgeID dyn_graph_access::get_first_edge(NodeID node) { return 0; }

EdgeID dyn_graph_access::get_first_invalid_edge(NodeID node) { return m_edges[node].size(); }

EdgeID dyn_graph_access::getNodeDegree(NodeID node) { return m_edges[node].size(); }

NodeID dyn_graph_access::getEdgeTarget(NodeID source, EdgeID edge) {
    ASSERT_TRUE(m_edges.size() > 0);
    ASSERT_TRUE(m_edges[source].size() > edge);

    return m_edges[source][edge].target;
}


#endif /* noend of include guard: dyn_graph_access_EFRXO4X2 */
