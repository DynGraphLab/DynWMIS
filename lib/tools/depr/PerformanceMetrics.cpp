//
// Created by Jannick Borowitz on 03.12.21.
//

#include "PerformanceMetrics.h"
#include "../definitions.h"

PerformanceMetrics::PerformanceMetrics(std::size_t measurePointSize) {
    //measurePoints.resize(measurePointSize, {0, 0, 0, 0, 0, 0});

    std::size_t counter = 0;

    if(measurePointSize > 0){
        measurePoints.resize(measurePointSize, {0, 0, 0, 0, 0, 0});
        for (auto& measurePoint : measurePoints) {
            measurePoint.measurePoint = counter;
        }

        currentMeasurePoint = begin();
    }
}

PerformanceMetrics::iterator PerformanceMetrics::begin() { return measurePoints.begin(); }

PerformanceMetrics::iterator PerformanceMetrics::end() { return measurePoints.end(); }

PerformanceMetrics::MeasurePoint& PerformanceMetrics::current() { return *currentMeasurePoint; }

void PerformanceMetrics::next() {
    ++currentMeasurePoint;
    if (currentMeasurePoint == end()) {
        measurePoints.push_back({0,0,0,0,0,0});
        currentMeasurePoint = --measurePoints.end();
    }
}

void PerformanceMetrics::newMeasurePoint() {
   measurePoints.push_back({0,0,0,0,0,0});
   /*if(measurePoints.size() == 1) {
       currentMeasurePoint = measurePoints.begin();
   }else{
       ++currentMeasurePoint;
       auto c = currentMeasurePoint;
       ++c;
       DYNWMIS_ASSERT(c!=measurePoints.end());
       ++c;
       DYNWMIS_ASSERT(c==measurePoints.end());
   }*/
   currentMeasurePoint = --measurePoints.end();

}
size_t PerformanceMetrics::size() { return measurePoints.size(); }
bool PerformanceMetrics::isMeasuring() { return is_measuring; }
bool PerformanceMetrics::startMeasuring() { return is_measuring=true; }
bool PerformanceMetrics::stopMeasuring() { return is_measuring=false; }
