//
//

#ifndef DELTA_WMIS_H
#define DELTA_WMIS_H

#include <memory>
#include <utility>

#include "DynWMISConfig.h"
#include "dyn_graph_access.h"
#include "random_functions.h"

class dyn_wmis_algorithm {
        public:

                virtual ~dyn_wmis_algorithm() = default;

                /**
                 * Notify local search solver about the edge insertion
                 * @param source
                 * @param target
                 */
                virtual void handleInsertion(NodeID source, NodeID target) = 0;

                /**
                 * Notify local search solver about the edge deletion
                 * @param source
                 * @param target
                 */
                virtual void handleDeletion(NodeID source, NodeID target) = 0;

                // weight of the current IS
                virtual NodeWeight weight() = 0;
                virtual std::vector<bool> getMIS() = 0;

                /**
                 * Doing some postprocessing once the overall graph has been read
                 */
                virtual void end() = 0;
                virtual void final() {};

                std::shared_ptr<dyn_graph_access> G;
                DynWMISConfig config;

        protected:
                dyn_wmis_algorithm(std::shared_ptr<dyn_graph_access> dyn_graph, DynWMISConfig config) : G(std::move(dyn_graph)), config(std::move(config)) {}

};

#endif  
