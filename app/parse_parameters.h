/******************************************************************************
 * parse_parameters.h 
 * *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 * Christian Schulz <christian.schulz.phone@gmail.com>
 *****************************************************************************/


#ifndef PARSE_PARAMETERS_GPJMGSM8
#define PARSE_PARAMETERS_GPJMGSM8

#include <omp.h>
#include <string>
#include <cstring>
#include <sstream>
#include "argtable3.h"
#include "DynWMISConfig.h"

int parse_parameters(int argn, char **argv, 
                DynWMISConfig & config, 
                std::string & graph_filename) {

        const char *progname = argv[0];

        // Setup argtable parameters.
        struct arg_lit *help                                 = arg_lit0(NULL, "help","Print help.");

        struct arg_str *filename                             = arg_strn(NULL, NULL, "FILE", 1, 1, "Path to dynamic graph file to compute independent set of.");
        struct arg_str *filename_output                      = arg_str0(NULL, "output_filename", NULL, "Specify the name of the output file (that contains the independent set).");
        struct arg_int *user_seed                            = arg_int0(NULL, "seed", NULL, "Seed to use for the PRNG.");
        struct arg_rex *algorithm                            = arg_rex0(NULL, "algorithm", "^(DynamicOneFast|DynamicOneStrong|DegGreedy|STATIC)$", "ALGORITHM", 1, "Algorithm to use. One of {DynamicOneFast, DynamicOneStrong, DegGreedy, Static}. Default: DynamicOneFast"  );
        struct arg_dbl *local_solver_time_limit              = arg_dbl0(NULL, "local_solver_time_limit", NULL, "Time limit of local solver.");
        struct arg_int *bfs_depth                            = arg_int0(NULL, "bfs_depth", NULL, "Depth of BFS in dynamic algorithm BFS. Default: 2");
        struct arg_int *local_problem_size_limit             = arg_int0(NULL, "local_problem_size_limit", NULL, "Max number of nodes in local problem. Default: Inf");

        struct arg_end *end                                  = arg_end(100);


        // Define argtable.
        void* argtable[] = {
                help, filename, filename_output, user_seed, algorithm, local_solver_time_limit, bfs_depth, local_problem_size_limit, end
        };
        // Parse arguments.
        int nerrors = arg_parse(argn, argv, argtable);

        // Catch case that help was requested.
        if (help->count > 0) {
                printf("Usage: %s", progname);
                arg_print_syntax(stdout, argtable, "\n");
                arg_print_glossary(stdout, argtable,"  %-40s %s\n");
                arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
                return 1;
        }


        if (nerrors > 0) {
                arg_print_errors(stderr, end, progname);
                printf("Try '%s --help' for more information.\n",progname);
                arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
                return 1; 
        }

        if(filename->count > 0) {
                graph_filename = filename->sval[0];
        }

        if(filename_output->count > 0) {
                config.output_filename = filename_output->sval[0];
        }

        if (user_seed->count > 0) {
                config.seed = user_seed->ival[0];
        }

        config.bfs_depth = 10;
        config.local_solver_time_limit = 1000;
        config.local_problem_size_limit = std::numeric_limits< int >::max();
        config.algorithmType = DYNWMIS_BFS;
        if (algorithm->count) {
                if(strcmp("DegGreedy", algorithm->sval[0]) == 0) {
                        config.algorithmType = DYNWMIS_GREEDY_DEG;
                } else  if(strcmp("DynamicOneFast", algorithm->sval[0]) == 0) {
                        config.algorithmType = DYNWMIS_BFS;
                        config.prune_updates = true;
                        config.rare_update_counter = 3;
                        config.rare_update_factor = 1.2;
                        config.limit_degree_parameter_factor = 1.35;
                        config.local_solver_time_limit = 10;
                        config.local_problem_size_limit = 200;
                } else  if(strcmp("DynamicOneStrong", algorithm->sval[0]) == 0) {
                        config.algorithmType = DYNWMIS_BFS;
                        config.prune_updates = true;
                        config.local_solver_time_limit = 10;
                        config.limit_degree_parameter_factor = 1.35;
                        config.local_problem_size_limit = 2500;
                } else  if(strcmp("STATIC", algorithm->sval[0]) == 0) {
                        config.algorithmType = DYNWMIS_STATIC;
                } else {
                        fprintf(stderr, "Invalid algorithm: \"%s\"\n", algorithm->sval[0]);
                        exit(0);
                }
        }

        config.local_algorithmType = MIS_BRANCH_AND_REDUCE;

        if(local_solver_time_limit->count) {
               config.local_solver_time_limit = local_solver_time_limit->dval[0];
        }
        config.kamis_wmis.time_limit = config.local_solver_time_limit;
        config.onlinemis.time_limit = config.local_solver_time_limit;

        if(bfs_depth->count) {
               config.bfs_depth = bfs_depth->ival[0];
        }

        if(local_problem_size_limit->count) {
                config.local_problem_size_limit = local_problem_size_limit->ival[0];
        }
        return 0;
}

#endif /* end of include guard: PARSE_PARAMETERS_GPJMGSM8 */
