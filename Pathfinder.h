#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include "GridGraph.h"

struct RouteState {
    int nodeId;
    double currentTotalCost;
    bool operator>(const RouteState& other) const {
        return currentTotalCost > other.currentTotalCost;
    }
};

class Pathfinder {
public:
    static double calculateSpecificRouteCost(GridGraph& network, const std::vector<int>& routeNodes) {
        double totalCost = 0.0;
        for (size_t i = 0; i < routeNodes.size() - 1; ++i) {
            int current = routeNodes[i];
            int next = routeNodes[i+1];
            bool edgeFound = false;
            
            for (const auto& edge : network.getOutgoingEdges(current)) {
                if (edge->destinationNodeId == next) {
                    totalCost += edge->totalCompositeWeight;
                    edgeFound = true;
                    break;
                }
            }
            if (!edgeFound) return -1.0; 
        }
        return totalCost;
    }

    static void findOptimalRoute(GridGraph& network, int startNodeId, int targetNodeId) {
        std::priority_queue<RouteState, std::vector<RouteState>, std::greater<RouteState>> openSet;
        std::unordered_map<int, double> lowestCostToReach;
        std::unordered_map<int, int> pathHistory;

        lowestCostToReach[startNodeId] = 0.0;
        openSet.push({startNodeId, 0.0});

        bool targetReached = false;

        while (!openSet.empty()) {
            RouteState currentState = openSet.top();
            openSet.pop();
            int currentNode = currentState.nodeId;

            if (currentNode == targetNodeId) {
                targetReached = true;
                break;
            }

            if (currentState.currentTotalCost > lowestCostToReach[currentNode]) continue;

            for (const auto& edge : network.getOutgoingEdges(currentNode)) {
                int neighborNode = edge->destinationNodeId;
                double tentativeCost = currentState.currentTotalCost + edge->totalCompositeWeight;

                if (lowestCostToReach.find(neighborNode) == lowestCostToReach.end() || 
                    tentativeCost < lowestCostToReach[neighborNode]) {
                    
                    lowestCostToReach[neighborNode] = tentativeCost;
                    pathHistory[neighborNode] = currentNode;
                    openSet.push({neighborNode, tentativeCost});
                }
            }
        }

        if (targetReached) {
            std::cout << "\n[ALGORITHM]: Optimal Route Discovered!" << std::endl;
            std::cout << "Optimized Project Cost: Rs. " << lowestCostToReach[targetNodeId] << std::endl;
            // WE NOW PASS THE NETWORK DATA INTO THE PRINT FUNCTION
            printPath(network, pathHistory, targetNodeId);
        } else {
            std::cout << "\n[ALGORITHM]: CRITICAL FAILURE. No viable route exists." << std::endl;
        }
    }

private:
    // WE ADDED THE 'GridGraph& network' SO IT CAN LOOK UP THE TEXT NAMES
    static void printPath(GridGraph& network, std::unordered_map<int, int>& pathHistory, int targetNodeId) {
        std::vector<int> finalPath;
        int current = targetNodeId;
        while (pathHistory.find(current) != pathHistory.end()) {
            finalPath.push_back(current);
            current = pathHistory[current];
        }
        finalPath.push_back(current);
        std::reverse(finalPath.begin(), finalPath.end());

        std::cout << "Optimized Routing Sequence: ";
        for (size_t i = 0; i < finalPath.size(); ++i) {
            // HERE IS THE MAGIC: Printing the actual city name!
            std::cout << network.getNode(finalPath[i])->cityName;
            if (i < finalPath.size() - 1) std::cout << " -> ";
        }
        std::cout << std::endl;
    }
};

#endif