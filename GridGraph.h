#ifndef GRID_GRAPH_H
#define GRID_GRAPH_H

#include <unordered_map>
#include <vector>
#include <memory>
#include "TerrainNode.h"
#include "TransmissionEdge.h"
#include "HeuristicEngine.h" // We are now connecting the math engine!

class GridGraph {
private:
    std::unordered_map<int, std::shared_ptr<TerrainNode>> nodes;
    std::unordered_map<int, std::vector<std::shared_ptr<TransmissionEdge>>> adjacencyList;

public:
    void addNode(std::shared_ptr<TerrainNode> node) {
        nodes[node->nodeId] = node;
    }

    void addTransmissionEdge(std::shared_ptr<TransmissionEdge> edge) {
        adjacencyList[edge->sourceNodeId].push_back(edge);
    }

    std::shared_ptr<TerrainNode> getNode(int id) {
        if (nodes.find(id) != nodes.end()) {
            return nodes[id];
        }
        return nullptr;
    }

    const std::vector<std::shared_ptr<TransmissionEdge>>& getOutgoingEdges(int nodeId) {
        return adjacencyList[nodeId];
    }

    int getNodeCount() const { 
        return nodes.size(); 
    }

    // THE NEW PIPELINE: This calculates the true cost for every single route
    void evaluateNetworkWeights() {
        for (auto& pair : adjacencyList) {
            int srcId = pair.first;
            auto srcNode = nodes[srcId];
            
            for (auto& edge : pair.second) {
                auto destNode = nodes[edge->destinationNodeId];
                if (srcNode && destNode) {
                    edge->totalCompositeWeight = HeuristicEngine::calculateEdgeWeight(srcNode, destNode, edge);
                }
            }
        }
    }
};

#endif