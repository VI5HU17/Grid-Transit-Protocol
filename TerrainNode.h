#ifndef TERRAIN_NODE_H
#define TERRAIN_NODE_H

// Categorizes the type of land (impacts legal friction)
enum class LandGrade {
    BARREN_GOVERNMENT,
    INDUSTRIAL_ZONE,
    SINGLE_CROP_FARM,
    MULTI_CROP_FARM,
    URBAN_RESIDENTIAL
};

// Categorizes the state laws (impacts compensation multipliers)
enum class StateJurisdiction {
    GUJARAT,
    RAJASTHAN,
    MAHARASHTRA
};

class TerrainNode {
public:
    int nodeId;
    double latitude;
    double longitude;
    double elevationMeters; 
    LandGrade grade;
    StateJurisdiction jurisdiction;
    double protestProbabilityScore; // 0.0 (Safe) to 1.0 (Guaranteed Protest)

    // Constructor to build the node
    TerrainNode(int id, double lat, double lon, double elev, LandGrade lg, StateJurisdiction j, double protestScore)
        : nodeId(id), latitude(lat), longitude(lon), elevationMeters(elev), 
          grade(lg), jurisdiction(j), protestProbabilityScore(protestScore) {}
};

#endif