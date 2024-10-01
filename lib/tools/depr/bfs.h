//
// Created by Jannick Borowitz on 12.04.22.
// This is the BFS used in the algorithms
//

#ifndef DYNWMIS_BFS_H
#define DYNWMIS_BFS_H

#include "vector"
#include "queue"

/*
 * bfs_with_radius
 * @param maxRadius
 * @param toVisit must always contain all expanded (radius, value) pairs; you have to initialize it;
 * @param onVisit processes the popped value
 * @param onReachedRadius allows to process a value if it was expanded at maxRadius
 * @param onExpandCurrentValue allows expanding around a value; it must implement that a value is not visited twice
 */
template <typename Value>
void bfs_within_radius(unsigned maxRadius, std::queue<std::pair<unsigned, Value>>& toVisit,
                       const std::function<bool(Value&)> onVisit,
                       const std::function<void(Value&)> onReachedMaxRadius,
                       const std::function<void(unsigned&, Value&)> onExpandCurrentValue) {
    while (!toVisit.empty()) {
        auto [radius, value] = toVisit.front();
        toVisit.pop();

        // if onVisit return true, the bfs should stop
        if (onVisit(value)) return;

        if (radius >= maxRadius) {
            onReachedMaxRadius(value);
        } else {
            onExpandCurrentValue(radius, value);
        }
    }
}

#endif  // DYNWMIS_BFS_H
