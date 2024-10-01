//
// Created by Jannick Borowitz on 03.12.21.
//

#ifndef DYNWMIS_PERFORMANCEMETRICS_H
#define DYNWMIS_PERFORMANCEMETRICS_H


#include <vector>
#include "definitions.h"

class PerformanceMetrics {
public:
    // types
    struct MeasurePoint {
        // NOTE here we can add additional fields for things we want to measure
        std::size_t measurePoint = 0;
        NodeWeight dynSolutionSize = 0;
        NodeWeight cmpStatic = 0;
        NodeWeight cmpStaticOfDynSolver = 0;
        std::size_t graphUpdate = 0;
        NodeWeight nodeSizeH = -1; // -1 means that it was not set yet
    };
    using iterator = std::vector<PerformanceMetrics::MeasurePoint>::iterator;

    // methods
    explicit PerformanceMetrics(std::size_t measurePointSize);
    iterator begin();
    iterator end();
    MeasurePoint &current();
    void next();
    size_t size();

    // general metrics
    double measureTimeGraphOperations = 0;
    double measureTimeUpdateOperations = 0;

    NodeWeight initialWISSize = 0;

    bool isMeasuring();
    bool startMeasuring();
    bool stopMeasuring();

    void newMeasurePoint();

   protected:
    // Properties
    std::vector<MeasurePoint> measurePoints;
    iterator currentMeasurePoint;
    bool is_measuring = true;
};


#endif //DYNWMIS_PERFORMANCEMETRICS_H
