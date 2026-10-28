#include "Fox.h"
#include "../World.h"
#include "../Random.h"
#include <vector>

Fox::Fox(Position pos)
    : Animal(pos, 3, 7, 3) {}

char Fox::getSymbol() const {
    return 'F';
}

std::string Fox::getSpecies() const {
    return "Fox";
}

bool Fox::findNewPosition(World& world, Position& targetPos) {
    const int range = getMovementRange();
    std::vector<Position> safeCandidates;
    safeCandidates.reserve((2 * range + 1) * (2 * range + 1) - 1);

    for (int dy = -range; dy <= range; ++dy) {
        for (int dx = -range; dx <= range; ++dx) {
            if (dy == 0 && dx == 0) {
                continue;
            }

            int targetY = getY() + dy;
            int targetX = getX() + dx;

            if (world.isWithinBounds(targetY, targetX)) {
                Organism* occupant = world.getOrganism(targetY, targetX);
                // Fox never enters a field occupied by a stronger organism
                if (occupant == nullptr || occupant->getStrength() <= getStrength()) {
                    safeCandidates.push_back({targetY, targetX});
                }
            }
        }
    }

    if (safeCandidates.empty()) {
        return false; // Surrounded by stronger predators, remains safe in place
    }

    targetPos = safeCandidates[Random::getInt(0, static_cast<int>(safeCandidates.size()) - 1)];
    return true;
}
