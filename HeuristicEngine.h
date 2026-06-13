#ifndef HEURISTIC_ENGINE_H
#define HEURISTIC_ENGINE_H

#include <cmath>
#include <memory>
#include "TerrainNode.h"
#include "TransmissionEdge.h"

class HeuristicEngine {
public:
    // Constants matching Indian Ministry of Power & GETCO infrastructure metrics
    static constexpr double TOWER_BASE_AREA_SQM = 120.0;     // Footprint of a 765kV tower base
    static constexpr double CORRIDOR_WIDTH_METERS = 46.0;    // Right of Way (RoW) corridor width width
    static constexpr double BASE_MATERIAL_COST_PER_KM = 2500000.0; // ₹25 Lakhs per km baseline wire cost
    static constexpr double STRANDED_ENERGY_BLEED_PER_DAY = 500000.0; // ₹5 Lakhs daily loss if generation is bottled up
    static constexpr double ELEVATION_STRAIN_MULTIPLIER = 15000.0; // Structural cost multiplier per vertical meter delta

    static double calculateEdgeWeight(const std::shared_ptr<TerrainNode>& src, 
                                      const std::shared_ptr<TerrainNode>& dest, 
                                      const std::shared_ptr<TransmissionEdge>& edge) {
        
        // 1. PHYSICAL ENGINEERING COST
        // Standard distance billing + heavy penalty for extreme elevation slopes
        double physicalCost = (edge->distanceKm * BASE_MATERIAL_COST_PER_KM) + 
                               (std::abs(edge->elevationDelta) * ELEVATION_STRAIN_MULTIPLIER);

        // 2. LEGAL COMPENSATION COST (Based on Land Grade and Circle Rates)
        double landValuePerSqm = getBaselineLandValue(dest->grade);
        
        // Adjust baseline calculations based on State Regulatory Jurisdictions
        double stateMultiplier = 1.0;
        if (dest->jurisdiction == StateJurisdiction::GUJARAT) {
            stateMultiplier = 1.15; // Gujarat-specific GETCO amendment increasing payout margins
        } else if (dest->jurisdiction == StateJurisdiction::MAHARASHTRA) {
            stateMultiplier = 1.10;
        }

        // 85% land value for the tower base plot + 15% value for the wide corridor strip (MoP norms)
        double baseCompensation = TOWER_BASE_AREA_SQM * landValuePerSqm * 0.85;
        double corridorAreaSqm = (edge->distanceKm * 1000.0) * CORRIDOR_WIDTH_METERS;
        double corridorCompensation = corridorAreaSqm * landValuePerSqm * 0.15;
        
        double legalCost = (baseCompensation + corridorCompensation) * stateMultiplier;

        // 3. SOCIOLOGICAL FRICTION COST (Financial risk modeling of farmer protests & court stays)
        double estimatedDelayDays = 0.0;
        if (dest->grade == LandGrade::MULTI_CROP_FARM) {
            estimatedDelayDays = 540.0; // ~1.5 years grid construction freeze risk
        } else if (dest->grade == LandGrade::SINGLE_CROP_FARM) {
            estimatedDelayDays = 180.0; // ~6 months intensive arbitration risk
        } else if (dest->grade == LandGrade::URBAN_RESIDENTIAL) {
            estimatedDelayDays = 1095.0; // 3 years intense legal/eminent domain gridlock
        }

        double frictionCost = dest->protestProbabilityScore * estimatedDelayDays * STRANDED_ENERGY_BLEED_PER_DAY;

        // Total algorithmic cost score representing capital and risk expenses combined
        return physicalCost + legalCost + frictionCost;
    }

private:
    static double getBaselineLandValue(LandGrade grade) {
        switch (grade) {
            case LandGrade::BARREN_GOVERNMENT: return 150.0;   // ₹150 per sq meter
            case LandGrade::INDUSTRIAL_ZONE:   return 1200.0;  // ₹1200 per sq meter
            case LandGrade::SINGLE_CROP_FARM:  return 800.0;   // ₹800 per sq meter
            case LandGrade::MULTI_CROP_FARM:   return 2500.0;  // ₹2500 per sq meter (Protected crop blocks)
            case LandGrade::URBAN_RESIDENTIAL: return 8000.0;  // ₹8000 per sq meter
            default: return 500.0;
        }
    }
};

#endif