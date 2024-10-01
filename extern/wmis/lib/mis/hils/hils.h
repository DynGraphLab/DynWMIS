//
// Created by alex on 22.03.20.
//

#ifndef COMPONENTS_HILS_H
#define COMPONENTS_HILS_H

#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "ReduceAndPeel.h"
#include "Solution.h"
#include "graph_access.h"
#include "graph_io.h"
#include "mis_config.h"
#include "timer.h"

std::mt19937 generator;
class hils {
// Mersenne Twister 19937 generator
private:
    timer t;
    const MISConfig &config;

public:
    hils(const MISConfig &config) : config(config) {}

    void perform_ils(graph_access &G, unsigned int max_iterations) {
        double p[4] = {2, 4, 4, 1}; // intensification/exploration parameters
        Solution s(&G);
        generator.seed(config.seed);

        forall_nodes(G, n)
            if (G.getPartitionIndex(n))
                s.addVertex(n);
        endfor
        while (!s.isMaximal()) {
            s.addRandomVertex();
        }
        // found initial solution
        do {
            while (!s.isMaximal()) {
                s.addRandomVertex();
            }
            if (t.elapsed() > config.time_limit) break;
        } while (s.omegaImprovement() || s.twoImprovement() );

        Solution best_s(s);

        // run ILS iterations

        int k = 1;
        int local_best = s.weight();
        int iter;
        for (iter = 0; t.elapsed() < config.time_limit && iter < max_iterations; iter++) {
            Solution next_s(s);

            // shake
            next_s.force(p[0]);

            assert(next_s.integrityCheck());

            do {
                while (!next_s.isMaximal()) {
                    next_s.addRandomVertex();
                }
            } while (next_s.omegaImprovement() || next_s.twoImprovement());

            assert(best_s.integrityCheck());

            if (next_s.weight() > s.weight()) {
                k = 1;
                s = next_s;

                if (local_best < next_s.weight()) {
                    k -= s.size() / p[1];
                    local_best = next_s.weight();
                }

                if (best_s.weight() < s.weight()) {
                    best_s = s;
                    k -= s.size() * p[2];
                }
            } else if (k <= s.size() / p[1]) {
                k++;
            } else {
                local_best = s.weight();
                s.force(p[3]);
                k = 1;
            }
        }

        NodeID i = 0;
        for (int state : best_s.solution()) {
            G.setPartitionIndex(i++, state);
        }
        assert(best_s.integrityCheck());
    }
};

#endif //COMPONENTS_HILS_H
