/******************************************************************************
 * configuration_mis.h
 *
 * Copyright (C) 2015-2017 Sebastian Lamm <lamm@ira.uka.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *****************************************************************************/

#ifndef _CONFIGURATION_MIS_H_
#define _CONFIGURATION_MIS_H_

#include "definitions.h"
#include "mis_config.h"

class configuration_mis {
   public:
    /**
     * Default Constructor.
     */
    configuration_mis(){};

    /**
     * Default Destructor.
     */
    virtual ~configuration_mis(){};

    /**
     * Set the standard configuration.
     *
     * @param config Config to be initialized.
     */
    void standard(MISConfig& config);
};

inline void configuration_mis::standard(MISConfig& mis_config) {
    // Basic
    mis_config.time_limit = 1000.0;
    mis_config.max_length_non_impr_seq = std::numeric_limits<size_t>::max();
    mis_config.compute_ils_on_lb_available = true;
    // Randomization
    mis_config.seed = 0;
    // Output
    mis_config.console_log = false;
    mis_config.check_sorted = true;
    // ILS
    mis_config.ils_iterations = 15000;
    mis_config.force_cand = 4;
    mis_config.sort_freenodes = true;
    // Reductions
    mis_config.perform_reductions = true;
    mis_config.reduction_style = MISConfig::Reduction_Style::NORMAL;
    // Weights
    mis_config.weight_source = MISConfig::Weight_Source::FILE;

    mis_config.set_limit = 1024;
    mis_config.struction_degree = 256;
    mis_config.struction_type = MISConfig::Struction_Type::EXTENDED;
    mis_config.key_type = MISConfig::Key_Type::APPROXIMATE_INCREASE;
    mis_config.key_reinsert_factor = 2;
    mis_config.global_blow_up_factor = 2;
    mis_config.phase_blow_up_factor = 2;
    mis_config.phase_blow_ups = 1;
    mis_config.max_unimproving_phases = 100;
    mis_config.backtrack_style = MISConfig::Backtrack_Type::IMMEDIATE_EXCLUDE;
    mis_config.reduce_and_peel = false;
    mis_config.generalized_fold_disabled = false;
    mis_config.neighborhood_clique_disabled = false;
    mis_config.critical_set_disabled = false;
    mis_config.clique_disabled = false;
    mis_config.blow_up_disabled = true;
    mis_config.plain_struction = false;
    mis_config.perform_hils = true;
}

#endif
