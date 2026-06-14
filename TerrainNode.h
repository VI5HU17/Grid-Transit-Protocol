#ifndef TERRAIN_NODE_H
#define TERRAIN_NODE_H

#include <string>

enum class LandGrade {
    BARREN_GOVERNMENT,
    INDUSTRIAL_ZONE,
    SINGLE_CROP_FARM,
    MULTI_CROP_FARM,
    URBAN_RESIDENTIAL
};

enum class StateJurisdiction {
    GUJARAT,
    RAJASTHAN,
    MAHARASHTRA
};

class TerrainNode {
public:
    int nodeId;
    std::string cityName; // NEW: The system now recognizes textual names
    double latitude;
    double longitude;
    double elevationMeters; 
    LandGrade grade;
    StateJurisdiction jurisdiction;
    double protestProbabilityScore; 

    // Updated Constructor
    TerrainNode(int id, std::string name, double lat, double lon, double elev, LandGrade lg, StateJurisdiction j, double protestScore)
        : nodeId(id), cityName(name), latitude(lat), longitude(lon), elevationMeters(elev), 
          grade(lg), jurisdiction(j), protestProbabilityScore(protestScore) {}
};

#endif