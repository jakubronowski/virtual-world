#include "Plant.h"
#include "World.h"
#include "Random.h"
#include <vector>

Plant::Plant(Position pos, int strength, int id)
    : Organism(pos, strength, 0, id) {}

void Plant::action(World& world) {
    if (Random::getInt(0, 99) < getSpreadChancePercent()) {
        std::vector<Position> freeSpots;
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dy == 0 && dx == 0) continue;

                int targetY = getY() + dy;
                int targetX = getX() + dx;

                if (world.isCellFree(targetY, targetX)) {
                    freeSpots.push_back({targetY, targetX});
                }
            }
        }

        if (!freeSpots.empty()) {
            Position spot = freeSpots[Random::getInt(0, static_cast<int>(freeSpots.size()) - 1)];
            world.addOffspring(getSpecies(), spot.y, spot.x);
            world.log("Germination: " + getSpecies() + " sprouted new vegetation at (" 
                      + std::to_string(spot.y) + ", " + std::to_string(spot.x) + ").");
        }
    }
}
