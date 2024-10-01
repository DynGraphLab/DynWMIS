#include <memory>

#include "dyn_wmis_simple.h"
#include "dyn_wmis_greedy.h"
#include "dyn_wmis_greedy_deg.h"
#include "dyn_wmis_bfs.h"
#include "dyn_wmis_static.h"

#ifndef DYNWMIS_FACTORY_H
#define DYNWMIS_FACTORY_H

template <typename... Args>
static std::unique_ptr<dyn_wmis_algorithm> getdyn_wmis_instance(DynWMISAlgorithmType algoType, Args&&... args) {
    std::unique_ptr<dyn_wmis_algorithm> solverPtr;
    switch (algoType) {
        case DYNWMIS_SIMPLE:
            solverPtr = std::make_unique<dyn_wmis_simple>(std::forward<Args>(args)...);
            break;
        case DYNWMIS_GREEDY:
            solverPtr = std::make_unique<dyn_wmis_greedy>(std::forward<Args>(args)...);
            break;
        case DYNWMIS_GREEDY_DEG:
            solverPtr = std::make_unique<dyn_wmis_greedy_deg>(std::forward<Args>(args)...);
            break;
        case DYNWMIS_BFS:
            solverPtr = std::make_unique<dyn_wmis_bfs>(std::forward<Args>(args)...);
            break;
        case DYNWMIS_STATIC:
            solverPtr = std::make_unique<dyn_wmis_static>(std::forward<Args>(args)...);
            break;

    }
    return solverPtr;
}

#endif  
