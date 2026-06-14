#ifndef MAX_FLOW_ENGINE_H
#define MAX_FLOW_ENGINE_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <limits>
#include "GridGraph.h"

class MaxFlowEngine {
public:
    // NEW: We added a 'printRoutes' boolean flag to turn the terminal output on/off
    static double calculateMaxGridCapacity(GridGraph& network, int sourceNodeId, int sinkNodeId, bool printRoutes = false) {
        std::unordered_map<int, std::unordered_map<int, double>> residualCapacity;
        
        for (const auto& nodePair : network.getAllNodes()) {
            int u = nodePair.first;
            for (const auto& edge : network.getOutgoingEdges(u)) {
                int v = edge->destinationNodeId;
                residualCapacity[u][v] += edge->maxMegawattCapacity;
                residualCapacity[v][u] += 0; 
            }
        }

        double maxTotalMegawatts = 0.0;
        std::unordered_map<int, int> parentPath;

        while (findAugmentingPathBFS(residualCapacity, sourceNodeId, sinkNodeId, parentPath)) {
            
            double pathFlowCapacity = std::numeric_limits<double>::max();
            int currentNode = sinkNodeId;
            
            // NEW: A list to track the exact cities the power is flowing through
            std::vector<int> activeRoute; 
            
            while (currentNode != sourceNodeId) {
                activeRoute.push_back(currentNode); // Record the node
                int previousNode = parentPath[currentNode];
                pathFlowCapacity = std::min(pathFlowCapacity, residualCapacity[previousNode][currentNode]);
                currentNode = previousNode;
            }
            activeRoute.push_back(sourceNodeId);
            std::reverse(activeRoute.begin(), activeRoute.end()); // Flip it so it reads Start -> End

            // NEW: If the print flag is ON, print the exact path to the executive dashboard
            if (printRoutes && pathFlowCapacity > 0) {
                std::cout << "   [+] Routing " << pathFlowCapacity << " MW via: ";
                for (size_t i = 0; i < activeRoute.size(); ++i) {
                    std::cout << network.getNode(activeRoute[i])->cityName;
                    if (i < activeRoute.size() - 1) std::cout << " -> ";
                }
                std::cout << std::endl;
            }

            // Standard capacity math
            currentNode = sinkNodeId;
            while (currentNode != sourceNodeId) {
                int previousNode = parentPath[currentNode];
                residualCapacity[previousNode][currentNode] -= pathFlowCapacity;
                residualCapacity[currentNode][previousNode] += pathFlowCapacity;
                currentNode = previousNode;
            }

            maxTotalMegawatts += pathFlowCapacity;
        }

        return maxTotalMegawatts;
    }

private:
    static bool findAugmentingPathBFS(
        std::unordered_map<int, std::unordered_map<int, double>>& residualCapacity, 
        int source, 
        int sink, 
        std::unordered_map<int, int>& parent) 
    {
        std::unordered_map<int, bool> visited;
        std::queue<int> q;
        
        q.push(source);
        visited[source] = true;
        parent[source] = -1;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (const auto& neighborPair : residualCapacity[current]) {
                int neighbor = neighborPair.first;
                double availableCapacity = neighborPair.second;

                if (!visited[neighbor] && availableCapacity > 0) {
                    q.push(neighbor);
                    parent[neighbor] = current;
                    visited[neighbor] = true;
                    
                    if (neighbor == sink) {
                        return true; 
                    }
                }
            }
        }
        return false; 
    }
};

#endif