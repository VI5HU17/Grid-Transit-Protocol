#include <iostream>
#include <memory>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include "TerrainNode.h"
#include "TransmissionEdge.h"
#include "GridGraph.h"
#include "Pathfinder.h" 
#include "MaxFlowEngine.h"
#include "DataLoader.h" // NEW: Connecting the File Parser

int findNodeIdByName(GridGraph& network, std::string inputName) {
    std::transform(inputName.begin(), inputName.end(), inputName.begin(), ::tolower);
    for (const auto& pair : network.getAllNodes()) {
        std::string nodeName = pair.second->cityName;
        std::transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::tolower);
        if (nodeName == inputName) return pair.first;
    }
    return -1; 
}

int main() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "==========================================================" << std::endl;
    std::cout << "  GATP: BIG DATA FILE I/O ENGINE INITIALIZED  " << std::endl;
    std::cout << "==========================================================\n" << std::endl;

    GridGraph network;

    // --- NEW: ABSTRACTION LAYER ---
    std::cout << "[SYSTEM]: Parsing CSV Data Files..." << std::endl;
    if (!DataLoader::loadNetworkData(network, "nodes.csv", "edges.csv")) {
        std::cout << "CRITICAL FAILURE: Could not load CSV data. Halting system." << std::endl;
        return 1;
    }
    std::cout << "[SYSTEM]: CSV Data Successfully Ingested." << std::endl;

    network.evaluateNetworkWeights();

    std::string startCityInput, targetCityInput;
    int startNode = -1, targetNode = -1;

    std::cout << "\nAvailable Grid Locations: Khavda, Bhuj, Morbi, Surendranagar, Ahmedabad\n" << std::endl;

    while (startNode == -1) {
        std::cout << "Type your STARTING location: ";
        std::getline(std::cin >> std::ws, startCityInput);
        startNode = findNodeIdByName(network, startCityInput);
        if (startNode == -1) std::cout << "[ERROR] City not found in grid. Check spelling.\n";
    }

    while (targetNode == -1) {
        std::cout << "Type your DESTINATION location: ";
        std::getline(std::cin >> std::ws, targetCityInput);
        targetNode = findNodeIdByName(network, targetCityInput);
        if (targetNode == -1) std::cout << "[ERROR] City not found in grid. Check spelling.\n";
    }

    std::cout << "\n==========================================================" << std::endl;
    std::cout << "  EXECUTIVE DASHBOARD" << std::endl;
    std::cout << "==========================================================" << std::endl;
    std::cout << "Routing Data: " << network.getNode(startNode)->cityName << " -> " << network.getNode(targetNode)->cityName << "\n" << std::endl;

    std::cout << "--- PHASE 1: FINANCIAL & ROUTING ANALYSIS ---" << std::endl;
    std::cout << "[SYSTEM]: Engaging A* Pathfinding Engine..." << std::endl;
    Pathfinder::findOptimalRoute(network, startNode, targetNode);

    std::cout << "\n--- PHASE 2: GRID RESILIENCE & FAILOVER ANALYSIS ---" << std::endl;
    double optimalCapacity = MaxFlowEngine::calculateMaxGridCapacity(network, startNode, targetNode);
    std::cout << "[SYSTEM STATUS]: GRID NOMINAL. All corridors operational." << std::endl;
    std::cout << "Maximum Safe Power Evacuation: " << optimalCapacity << " Megawatts\n" << std::endl;

    std::cout << " [CRITICAL ALERT]: PHYSICAL SHUTDOWN ON MAIN CORRIDOR DETECTED" << std::endl;
    
    // Programmatically find and disable the direct line (Khavda -> Morbi in this example)
    // In a full system, you would select this dynamically, but for the demo we find the 4000MW line and drop it
    for (const auto& edge : network.getOutgoingEdges(startNode)) {
        if (edge->maxMegawattCapacity == 4000.0) {
            edge->maxMegawattCapacity = 0.0;
        }
    }

    std::cout << "[SYSTEM]: Initiating Ford-Fulkerson Max-Flow Rerouting Protocol...\n" << std::endl;
    double failoverCapacity = MaxFlowEngine::calculateMaxGridCapacity(network, startNode, targetNode, true);
    
    std::cout << "\nEmergency Rerouted Power Evacuation: " << failoverCapacity << " Megawatts" << std::endl;
    double lostPower = optimalCapacity - failoverCapacity;
    std::cout << "Stranded Power (Requires VPP Storage Buffer): " << lostPower << " Megawatts" << std::endl;

    std::cout << "\n==========================================================" << std::endl;
    return 0;
}