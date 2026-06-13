#ifndef TRANSMISSION_EDGE_H
#define TRANSMISSION_EDGE_H

class TransmissionEdge {
public:
    int sourceNodeId;
    int destinationNodeId;
    
    // Physical constraints
    double distanceKm;
    double elevationDelta; // Change in height between nodes
    
    // Grid capacity constraints
    double maxMegawattCapacity;
    double currentLoad;
    
    // The final calculated cost (Physical + Legal + Sociological Friction)
    double totalCompositeWeight; 

    // Constructor to build the transmission line
    TransmissionEdge(int src, int dest, double dist, double elevDelta, double maxCap)
        : sourceNodeId(src), destinationNodeId(dest), distanceKm(dist), 
          elevationDelta(elevDelta), maxMegawattCapacity(maxCap), currentLoad(0.0), totalCompositeWeight(0.0) {}
};

#endif