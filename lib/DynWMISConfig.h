//
//

#ifndef DYNWMIS_H
#define DYNWMIS_H

#include <string>
#include <mis_config.h>

#include "configuration_mis.h"
#include "definitions_wmis.h"
#include "../extern/onlinemis/mis_config.h"
#include "../extern/onlinemis/configuration_mis.h"


class DynWMISConfig {
   public:

    int seed = 0;

    std::string output_filename;

    DynWMISAlgorithmType algorithmType;
    DynWMISLocalAlgorithmType local_algorithmType;
    DynWMISWeightType weightType;

    MISConfig kamis_wmis;
    onlinemis::MISConfig onlinemis;

    DynWMISConfig() {
            configuration_mis configurator_wmis;
            configurator_wmis.standard(kamis_wmis);
            kamis_wmis.time_limit                  = 1000;
            kamis_wmis.ils_iterations              = 100;
            kamis_wmis.seed                        = seed;
            kamis_wmis.max_length_non_impr_seq     = std::numeric_limits<size_t>::max(); // 100;
            kamis_wmis.compute_ils_on_lb_available = true;
            kamis_wmis.reduction_style             = MISConfig::Reduction_Style::NORMAL;

            onlinemis::configuration_mis configurator_omis;
            configurator_omis.standard(onlinemis);
            onlinemis.seed                  = seed;
            onlinemis.time_limit            = 1000;
            onlinemis.console_log           = false;
            onlinemis.print_log             = false;
            onlinemis.check_sorted          = false;
            onlinemis.start_greedy_adaptive = false;
            onlinemis.write_graph           = false;
            prune_updates                   = false;
            rare_update_counter   = 1;
            limit_degree_parameter_count = std::numeric_limits<int>::max();
            limit_degree_parameter_factor = 10000;
            rare_update_factor = 1;

    }

    double local_solver_time_limit;
    bool unit_weights;
    int bfs_depth;
    int local_problem_size_limit;
    bool prune_updates;
    int rare_update_counter; 
    int limit_degree_parameter_count; 
    double limit_degree_parameter_factor;
    double rare_update_factor;

};

#endif  
