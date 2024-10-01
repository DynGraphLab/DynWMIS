//
// Created by Jannick Borowitz on 18.04.22.
//

#include <random_functions.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "parse_parameters.h"
#include "DynamicAlgorithmMeta.h"
#include "DynWMISConfig.h"
#include "algorithm_factory.h"
#include "mwis_graph_io.h"
#include "timer.h"

int main(int argc, char** argv) {


        DynWMISConfig config;

        std::string graph_filename;
        int ret_code = parse_parameters(argc, argv, config, graph_filename);

        if(ret_code) {
                return 0;
        }


        srand(config.seed);
        random_functions::setSeed(config.seed);

        // read sequence
        auto graphIO = mwis_graph_io();
        timer t; t.restart();

        std::vector<std::pair<int, std::pair<NodeID, NodeID> > > edge_sequence; 
        bool has_node_weights = false;
        std::vector< NodeWeight > node_weights;
        std::pair<int,int> p = graphIO.readEdgeSequence( graph_filename, edge_sequence, node_weights, has_node_weights);
        int n = p.first;
        std::cout <<  "io took " <<  t.elapsed()  << std::endl; 

        //configure algorithm based on input parameters
        std::shared_ptr<dyn_graph_access> G = std::make_shared<dyn_graph_access>(n);

        std::shared_ptr<dyn_wmis_algorithm> algorithm = getdyn_wmis_instance(config.algorithmType, G, config);
        config.local_algorithmType = MIS_BRANCH_AND_REDUCE;
        if(has_node_weights) {
                forall_nodes((*G), node) {
                        G->setNodeWeight(node, node_weights[node]);
                } endfor
                config.local_algorithmType = BRANCH_AND_REDUCE;
        } else {
                forall_nodes((*G), node) {
                        G->setNodeWeight(node, 1);
                } endfor
        }

        t.restart();
        // run algorithm
        if(algorithm != NULL) 
                run_dynamic_algorithm(G, edge_sequence, algorithm);

        std::cout <<  "time \t\t " <<  t.elapsed()  << std::endl;
        std::cout <<  "weight \t\t " <<  algorithm->weight()  << std::endl;

        //call some postprocessing
        algorithm->end();

        std::vector< bool > mis;
        mis = algorithm->getMIS();
        std::shared_ptr<dyn_graph_access> G_bar = std::make_shared<dyn_graph_access>(n);
        int mis_size = 0;
        for (size_t i = 0; i < edge_sequence.size(); ++i) { 
                std::pair<NodeID, NodeID> & edge = edge_sequence[i].second;

                if (edge_sequence.at(i).first) {
                        G->new_edge(edge.first, edge.second);
                        G->new_edge(edge.second, edge.first);
                } else {
                        G->remove_edge(edge.first, edge.second);
                        G->remove_edge(edge.second, edge.first);
                }



        } 

        forall_nodes((*G), node) {
                if( mis[node] ) {
                        //std::cout <<  "degree of is node " <<  G->getNodeDegree(node)  << std::endl;
                        mis_size += G->getNodeWeight(node);
                        forall_out_edges((*G), e, node) {
                                NodeID target = G->getEdgeTarget(node, e);
                                if( mis[target] ) {
                                        std::cout <<  "NOT AN INDEPENDENT SET (" << node << "," << target << ")"  << std::endl;
                                        exit(0);
                                }
                        } endfor
                } else {
                        //std::cout <<  "degree of non-is node " <<  G->getNodeDegree(node)  << std::endl;
                }
        } endfor

        std::cout <<  "mis size checked is " <<  mis_size  << std::endl;

        return 0;
}
