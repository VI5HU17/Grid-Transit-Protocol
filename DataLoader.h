#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include "GridGraph.h"

class DataLoader {
public:
    static bool loadNetworkData(GridGraph& network, const std::string& nodesFile, const std::string& edgesFile) {
        if (!loadNodes(network, nodesFile)) return false;
        if (!loadEdges(network, edgesFile)) return false;
        return true;
    }

private:
    // Helper to convert text strings from the CSV into our C++ Enums
    static LandGrade parseLandGrade(const std::string& str) {
        if (str == "INDUSTRIAL_ZONE") return LandGrade::INDUSTRIAL_ZONE;
        if (str == "SINGLE_CROP_FARM") return LandGrade::SINGLE_CROP_FARM;
        if (str == "MULTI_CROP_FARM") return LandGrade::MULTI_CROP_FARM;
        if (str == "URBAN_RESIDENTIAL") return LandGrade::URBAN_RESIDENTIAL;
        return LandGrade::BARREN_GOVERNMENT; // Default
    }

    static StateJurisdiction parseJurisdiction(const std::string& str) {
        if (str == "RAJASTHAN") return StateJurisdiction::RAJASTHAN;
        if (str == "MAHARASHTRA") return StateJurisdiction::MAHARASHTRA;
        return StateJurisdiction::GUJARAT; // Default
    }

    static bool loadNodes(GridGraph& network, const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "[ERROR]: Could not open " << filename << std::endl;
            return false;
        }

        std::string line;
        std::getline(file, line);
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string idStr, name, latStr, lonStr, elevStr, gradeStr, jurStr, protestStr;

            // Parse by commas
            std::getline(ss, idStr, ',');
            std::getline(ss, name, ',');
            std::getline(ss, latStr, ',');
            std::getline(ss, lonStr, ',');
            std::getline(ss, elevStr, ',');
            std::getline(ss, gradeStr, ',');
            std::getline(ss, jurStr, ',');
            std::getline(ss, protestStr, ',');

            int id = std::stoi(idStr);
            double lat = std::stod(latStr);
            double lon = std::stod(lonStr);
            double elev = std::stod(elevStr);
            LandGrade grade = parseLandGrade(gradeStr);
            StateJurisdiction jur = parseJurisdiction(jurStr);
            double protest = std::stod(protestStr);

            network.addNode(std::make_shared<TerrainNode>(id, name, lat, lon, elev, grade, jur, protest));
        }
        return true;
    }

    static bool loadEdges(GridGraph& network, const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "[ERROR]: Could not open " << filename << std::endl;
            return false;
        }

        std::string line;
        std::getline(file, line);
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string srcStr, destStr, distStr, elevStr, capStr;

            std::getline(ss, srcStr, ',');
            std::getline(ss, destStr, ',');
            std::getline(ss, distStr, ',');
            std::getline(ss, elevStr, ',');
            std::getline(ss, capStr, ',');

            int src = std::stoi(srcStr);
            int dest = std::stoi(destStr);
            double dist = std::stod(distStr);
            double elev = std::stod(elevStr);
            double cap = std::stod(capStr);

            network.addTransmissionEdge(std::make_shared<TransmissionEdge>(src, dest, dist, elev, cap));
        }
        return true;
    }
};

#endif