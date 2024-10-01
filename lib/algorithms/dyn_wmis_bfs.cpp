//
//

#include <cmath>
#include <set>
#include "dyn_wmis_bfs.h"
#include "graph_io.h"
#include "solve_static_wmis.h"
#include "random_functions.h"
int counter = 0;

dyn_wmis_bfs::dyn_wmis_bfs(const std::shared_ptr<dyn_graph_access>& dyn_graph, const DynWMISConfig& config)
        : dyn_wmis_algorithm(dyn_graph, config) {
                m_is_IS.resize(dyn_graph->number_of_nodes());
                m_deepth.resize(dyn_graph->number_of_nodes());
                m_map_old_to_subgraphID.resize(dyn_graph->number_of_nodes());
                update_counter = 0;
                max_solution_node_degree = 5;
                last_source = std::numeric_limits<NodeID>::max();
                m_rare_update_counter = config.rare_update_counter;
                for( unsigned i = 0; i < m_is_IS.size(); i++) {
                        m_is_IS[i] = true;
                        m_deepth[i] = -1;
                        m_map_old_to_subgraphID[i] = -1;
                }
        }

void dyn_wmis_bfs::doBFSnSolve(NodeID source, NodeID target) {
        if(config.prune_updates) {
                bool success = false;
                if(!m_is_IS[source])  {
                        // check if any of the neighbors can go in
                        forall_out_edges((*G), e, source) {
                                NodeID target_ = G->getEdgeTarget(source, e);
                                bool is_free = true;
                                if(!m_is_IS[target_]) {
                                        forall_out_edges((*G), e_bar, target_) {
                                                NodeID target__ = G->getEdgeTarget(target_, e_bar);
                                                if(m_is_IS[target__]) {
                                                        is_free = false;
                                                        break;
                                                }
                                        } endfor
                                        if(is_free) {
                                                m_is_IS[target_] = true;
                                                success = true; 
                                        }
                                }
                        } endfor
                }
                if(!m_is_IS[target])  {
                        // check if any of the neighbors can go in
                        forall_out_edges((*G), e, target) {
                                NodeID target_ = G->getEdgeTarget(target, e);
                                bool is_free = true;
                                if(!m_is_IS[target_] ) {
                                        forall_out_edges((*G), e_bar, target_) {
                                                NodeID target__ = G->getEdgeTarget(target_, e_bar);
                                                if(m_is_IS[target__]) {
                                                        is_free = false;
                                                        break;
                                                }
                                        } endfor
                                        if(is_free) {
                                                m_is_IS[target_] = true;
                                                success = true; 
                                        }
                                }
                        } endfor
                }
                if(success) {return; }
        }

        if(last_source == source) { return;}
        last_source = source;

        if(!(update_counter % m_rare_update_counter == 0) ) {
                return;
        }
        // ******************* 
        // BFS
        // ******************* 
        std::queue< NodeID > bfsqueue;
        std::vector< NodeID > touched; //TODO: exchange set data structure
        std::vector< NodeID > undolist; //TODO: exchange set data structure
        bfsqueue.push(source);
        touched.push_back(source);
        undolist.push_back(source);
        m_deepth[source]  = 0;
        int cur_m_deepth  = 0;
        int node_counter  = 1;

        int degree_limit = config.limit_degree_parameter_count;
        if( config.limit_degree_parameter_count == std::numeric_limits<int>::max() ) {
                degree_limit = config.limit_degree_parameter_factor * max_solution_node_degree;
                //std::cout <<  "degree limit " <<  degree_limit  << std::endl;
        }
        while(!bfsqueue.empty() && node_counter < config.local_problem_size_limit) {
                NodeID node = bfsqueue.front();
                bfsqueue.pop(); 

                if (m_deepth[node] == cur_m_deepth) {
                        cur_m_deepth++;
                }
                if(cur_m_deepth > config.bfs_depth) {
                        break;
                }

                forall_out_edges((*G), e, node) {
                        NodeID target = G->getEdgeTarget(node,e);
                        if(m_deepth[target] == -1) {
                                m_deepth[target] = cur_m_deepth;
                                if((G->getNodeDegree(target) <= degree_limit)  || m_is_IS[target]) {
                                        touched.push_back(target);
                                        node_counter++;
                                } else {
                                        m_deepth[target] = std::numeric_limits<int>::max();
                                }
                                undolist.push_back(target);
                                bfsqueue.push(target);
                                if( node_counter >= config.local_problem_size_limit) { break; }
                        }
                } endfor
        }

        // ******************* 
        // Add adjacent IS nodes
        // ******************* 
        std::vector< NodeID > interim_set;
        for( NodeID node : touched ) {
                interim_set.push_back(node);
                forall_out_edges((*G), e, node) {
                        NodeID target = G->getEdgeTarget(node, e);
                        if( (m_deepth[target] == -1 && m_is_IS[node])) {
                                interim_set.push_back(target);
                                undolist.push_back(target);
                                m_deepth[target] = 0; // make sure we add the IS node only once
                        }
                } endfor

        }

        std::vector< NodeID > final_set;
        EdgeID upper_bound_edges = 0;
        for( NodeID node : interim_set) {
                final_set.push_back(node);
                upper_bound_edges += G->getNodeDegree(node);
                forall_out_edges((*G), e, node) {
                        NodeID target = G->getEdgeTarget(node, e);
                        if( m_deepth[target] == -1 ) {
                                // check if it is only adjacent to IS nodes in interim set
                                bool has_to_be_added = true;
                                forall_out_edges((*G), e_bar, target) {
                                        NodeID target_ = G->getEdgeTarget(target, e_bar);
                                        if( m_is_IS[target_] && m_deepth[target_] == -1 ) {
                                                // we don't need to add it
                                                has_to_be_added = false; 
                                                break;
                                        } 
                                } endfor
                                if( has_to_be_added ) {
                                        final_set.push_back(target);
                                        undolist.push_back(target);
                                        upper_bound_edges += G->getNodeDegree(target);
                                        m_deepth[target] = 0; // make sure we add the node only once
                                }
                        }
                } endfor

        }

        // ******************* 
        // next step: extract subgraph
        // ******************* 
        graph_access H;
        H.start_construction(final_set.size(), upper_bound_edges);

        NodeID counter = 0;
        for( NodeID u : final_set) {
                m_map_old_to_subgraphID[u] = counter++;
        }

        for( NodeID u : final_set) {
                NodeID cur_node = H.new_node();
                H.setNodeWeight(cur_node, G->getNodeWeight(u));
                forall_out_edges((*G), e, u) {
                        NodeID target = G->getEdgeTarget(u, e);
                        if( m_map_old_to_subgraphID[target] != -1 ) {
                                H.new_edge(cur_node, m_map_old_to_subgraphID[target]);
                        }
                } endfor
        }

        H.finish_construction();

        int old_mis = 0;
        for( NodeID u : final_set) {
                if(m_is_IS[u]) old_mis+= G->getNodeWeight(u);
        }

        //std::cout <<  H.number_of_nodes()  << std::endl;
        // next step: run solver 
        if( H.number_of_nodes() == 2) {
                if(H.getNodeWeight(0) > H.getNodeWeight(1)) {
                        H.setPartitionIndex(0, 1);
                        H.setPartitionIndex(1, 0);
                } else {
                        H.setPartitionIndex(0, 0);
                        H.setPartitionIndex(1, 1);
                }
        } else {
                //if( H.number_of_nodes() <= 4) {
                        //findOptimalIndependentSet(H);
                //} else {
                solve_static_wmis solver;
                solver.set_omis_by_unsucc_iterations(); // set stopping criterion in case of omis
                solver.solve_subgraph(config, H, old_mis);//}
        }

        int mis = 0; 
        forall_nodes(H, node) {
                if(H.getPartitionIndex(node) == 1) mis += H.getNodeWeight(node);
        } endfor

        if(old_mis < mis) {
                m_rare_update_counter = config.rare_update_counter;
                //max_solution_node_degree = 0;
                for( NodeID u : final_set) {
                        m_is_IS[u] = H.getPartitionIndex(m_map_old_to_subgraphID[u]);
                        if(m_is_IS[u]) {
                                if(G->getNodeDegree(u) > max_solution_node_degree) {
                                        max_solution_node_degree = G->getNodeDegree(u);
                                }
                        }
                }
        } else {
                m_rare_update_counter = ceil(m_rare_update_counter*config.rare_update_factor);
        }

        for( NodeID u : undolist) {
                m_deepth[u] = -1;
                m_map_old_to_subgraphID[u] = -1;
        }
}

void dyn_wmis_bfs::handleInsertion(NodeID source, NodeID target) {
        G->new_edge(source, target);
        G->new_edge(target, source);
        update_counter++;

        if(m_is_IS[source] && m_is_IS[target] ) {
                //double prio_source = G->getNodeWeight(source)/(double)G->getNodeDegree(source);
                //double prio_target = G->getNodeWeight(target)/(double)G->getNodeDegree(target);
                //if(prio_source < prio_target ) {
                        //m_is_IS[source] = false;
                //} else if( prio_source > prio_target ) {
                        //m_is_IS[target] = false;
                //} else {
                        //if(random_functions::nextBool()) {
                                //m_is_IS[source] = false;
                        //} else {
                                //m_is_IS[target] = false;
                        //}
                //}
                NodeWeight nweights = 0;
                forall_out_edges((*G), e, source) {
                        NodeID target_ = G->getEdgeTarget(source, e);
                        nweights += G->getNodeWeight(target_);
                } endfor
                NodeWeight nweightt = 0;
                forall_out_edges((*G), e, target) {
                        NodeID target_ = G->getEdgeTarget(target, e);
                        nweightt += G->getNodeWeight(target_);
                } endfor
                // the following should be equivalent to the above in cardinality case (using other inequalities below)
                double prio_source =  (double) G->getNodeWeight(source) / (double) nweights ;
                double prio_target = (double) G->getNodeWeight(target) / (double) nweightt  ; 


                if(prio_source < prio_target ) {  //> for card experiments in paper
                        m_is_IS[source] = false;
                } else if( prio_source > prio_target ) { //< for card experiments in paper
                        m_is_IS[target] = false;
                //double prio_source = nweights - G->getNodeWeight(source);
                //double prio_target = nweightt - G->getNodeWeight(target); 

                //if(prio_source > prio_target ) {
                        //m_is_IS[source] = false;
                //} else if( prio_source < prio_target ) {
                        //m_is_IS[target] = false;
                } else {
                        if(random_functions::nextBool()) {
                                m_is_IS[source] = false;
                        } else {
                                m_is_IS[target] = false;
                        }
                }
                doBFSnSolve(source, target);



        }
}

void dyn_wmis_bfs::handleDeletion(NodeID source, NodeID target) {
        G->remove_edge(source, target);
        G->remove_edge(target, source);
        bool s_can_be_added = true; 
        forall_out_edges((*G), e, source) {
                NodeID target_ = G->getEdgeTarget(source, e);
                if( m_is_IS[target_] ) {
                        s_can_be_added = false;
                        break;
                }
        } endfor

        bool t_can_be_added = true; 
        forall_out_edges((*G), e, target) {
                NodeID target_ = G->getEdgeTarget(target, e);
                if( m_is_IS[target_] ) {
                        t_can_be_added = false;
                        break;
                }
        } endfor

        if( s_can_be_added ) {
                m_is_IS[source] = true;
        }

        if( t_can_be_added ) {
                m_is_IS[target] = true;
        }

        if( !(s_can_be_added && t_can_be_added) ) {
                doBFSnSolve(source, target);
        }
}


// Function to check if a subset of nodes forms an independent set
bool dyn_wmis_bfs::isIndependentSet(graph_access& G, const std::vector<int>& subset) {
    for (size_t i = 0; i < subset.size(); ++i) {
        for (size_t j = i + 1; j < subset.size(); ++j) {
                forall_out_edges(G, e, subset[i]) {
                        NodeID target = G.getEdgeTarget(e);
                        if(target == subset[j]) {
                                return false;
                        }
                } endfor
        }
    }
    return true;
}

// Function to find the total weight of a subset of nodes
int dyn_wmis_bfs::totalWeight(graph_access& G, const std::vector<int>& subset) {
    int weight = 0;
    for (int node : subset) {
        weight += G.getNodeWeight(node);
    }
    return weight;
}

// Function to find all subsets of a set
void dyn_wmis_bfs::findSubsets(const std::vector<int>& set, std::vector<std::vector<int>>& subsets) {
    int n = set.size();
    int subsetCount = 1 << n; // 2^n subsets
    for (int i = 0; i < subsetCount; ++i) {
        std::vector<int> subset;
        for (int j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.push_back(set[j]);
            }
        }
        subsets.push_back(subset);
    }
}

// Function to find the optimal weighted independent set by exhaustive enumeration
void dyn_wmis_bfs::findOptimalIndependentSet(graph_access& G) {
    int n = G.number_of_nodes();
    std::vector<int> nodes(n);
    for (int i = 0; i < n; ++i) {
        nodes[i] = i;
    }

    std::vector<std::vector<int>> subsets;
    findSubsets(nodes, subsets);

    std::vector<int> optimalSet;
    int maxWeight = std::numeric_limits<int>::min();

    for (const auto& subset : subsets) {
        if (isIndependentSet(G, subset)) {
            int weight = totalWeight(G, subset);
            if (weight > maxWeight) {
                maxWeight = weight;
                optimalSet = subset;
            }
        }
    }
    forall_nodes(G, node) {
            G.setPartitionIndex(node, 0);
    } endfor

    
    for(int node : optimalSet) {
            G.setPartitionIndex(node, 1);
    }
    

}

//int main() {
    //// Assuming graph_access is initialized and populated
    //graph_access G;
    //// Initialization code for G...

    //std::vector<int> optimalSet = findOptimalIndependentSet(G);

    //std::cout << "Optimal Independent Set: ";
    //for (int node : optimalSet) {
        //std::cout << node << " ";
    //}
    //std::cout << std::endl;

    //int maxWeight = totalWeight(G, optimalSet);
    //std::cout << "Maximum Weight: " << maxWeight << std::endl;

    //return 0;
//}
