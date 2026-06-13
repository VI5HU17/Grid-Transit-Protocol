#include <iostream>
#include <memory>
#include <iomanip>
#include <vector>
#include "TerrainNode.h"
#include "TransmissionEdge.h"
#include "GridGraph.h"
#include "Pathfinder.h" 

int main() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "==========================================================" << std::endl;
    std::cout << "  GATP: GRID ABSORPTION & TRANSIT PROTOCOL INITIALIZED  " << std::endl;
    std::cout << "==========================================================\n" << std::endl;

    GridGraph network;

    std::cout << "[SYSTEM]: Loading Geographic Nodes..." << std::endl;
    network.addNode(std::make_shared<TerrainNode>(1, 23.85, 69.73, 15.0, LandGrade::BARREN_GOVERNMENT, StateJurisdiction::GUJARAT, 0.02)); // Khavda
    network.addNode(std::make_shared<TerrainNode>(2, 23.25, 69.66, 110.0, LandGrade::BARREN_GOVERNMENT, StateJurisdiction::GUJARAT, 0.05)); // Bhuj
    network.addNode(std::make_shared<TerrainNode>(3, 22.81, 70.83, 54.0, LandGrade::MULTI_CROP_FARM, StateJurisdiction::GUJARAT, 0.88)); // Morbi
    network.addNode(std::make_shared<TerrainNode>(4, 22.72, 71.63, 65.0, LandGrade::SINGLE_CROP_FARM, StateJurisdiction::GUJARAT, 0.35)); // Surendranagar
    network.addNode(std::make_shared<TerrainNode>(5, 23.02, 72.57, 53.0, LandGrade::URBAN_RESIDENTIAL, StateJurisdiction::GUJARAT, 0.10)); // Ahmedabad

    std::cout << "[SYSTEM]: Mapping Potential Corridors..." << std::endl;
    
    // Routes
    network.addTransmissionEdge(std::make_shared<TransmissionEdge>(1, 3, 120.0, 39.0, 2000.0)); // Khavda -> Morbi
    network.addTransmissionEdge(std::make_shared<TransmissionEdge>(3, 5, 190.0, -1.0, 2000.0)); // Morbi -> Ahmedabad
    network.addTransmissionEdge(std::make_shared<TransmissionEdge>(1, 2, 70.0, 95.0, 2000.0));  // Khavda -> Bhuj
    network.addTransmissionEdge(std::make_shared<TransmissionEdge>(2, 4, 210.0, -45.0, 2000.0)); // Bhuj -> Surendranagar
    network.addTransmissionEdge(std::make_shared<TransmissionEdge>(4, 5, 130.0, -12.0, 2000.0)); // Surendranagar -> Ahmedabad
    network.addTransmissionEdge(std::make_shared<TransmissionEdge>(2, 3, 115.0, -56.0, 2000.0)); // Bhuj -> Morbi
    network.addTransmissionEdge(std::make_shared<TransmissionEdge>(3, 4, 85.0, 11.0, 2000.0));  // Morbi -> Surendranagar

    std::cout << "[SYSTEM]: Executing Heuristic Weight Calculations..." << std::endl;
    network.evaluateNetworkWeights();

    int startNode, targetNode;
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "  INTERACTIVE ROUTING TERMINAL" << std::endl;
    std::cout << "==========================================================" << std::endl;
    std::cout << " [1] Khavda (Generation Site)\n [2] Bhuj (Transit Node)\n [3] Morbi (High-Risk Farming Zone)\n [4] Surendranagar (Transit Node)\n [5] Ahmedabad (Demand Hub)" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    std::cout << "Enter Start Node ID (1-5): ";
    std::cin >> startNode;

    std::cout << "Enter Destination Node ID (1-5): ";
    std::cin >> targetNode;

    std::cout << "\n==========================================================" << std::endl;
    std::cout << "  EXECUTIVE COMPARATIVE ANALYSIS" << std::endl;
    std::cout << "==========================================================" << std::endl;

    // The Pitch: If they are doing a full cross-state Khavda to Ahmedabad route
    if (startNode == 1 && targetNode == 5) {
        std::vector<int> directPath = {1, 3, 5}; // The path through the high-risk farms
        double directCost = Pathfinder::calculateSpecificRouteCost(network, directPath);
        
        std::cout << "[BASELINE]: Traditional Direct Route (Node 1 -> Node 3 -> Node 5)" << std::endl;
        std::cout << "Baseline Cost Estimate: Rs. " << directCost << std::endl;
        std::cout << "Risk Profile: CRITICAL (Severe multi-crop farmer protests expected at Node 3)\n" << std::endl;
    }

    std::cout << "[SYSTEM]: Engaging Dijkstra / A* Pathfinding Engine..." << std::endl;
    Pathfinder::findOptimalRoute(network, startNode, targetNode);

    std::cout << "\n==========================================================" << std::endl;
    return 0;
}