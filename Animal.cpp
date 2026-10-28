#include "Animal.h"
#include "World.h"
#include "Random.h"

Animal::Animal(Position pos, int strength, int initiative, int id)
    : Organism(pos, strength, initiative, id) {}

void Animal::action(World& world) {
    Position targetPos;
    if (!findNewPosition(world, targetPos)) {
        return; // No valid move possible
    }

    if (world.isCellFree(targetPos.y, targetPos.x)) {
        world.moveOrganism(getPosition(), targetPos);
    } else {
        Organism* defender = world.getOrganism(targetPos.y, targetPos.x);
        if (defender) {
            collideWith(*defender, world);
        }
    }
}

void Animal::collideWith(Organism& other, World& world) {
    // 1. Same species interaction: Reproduction
    if (getSpecies() == other.getSpecies()) {
        std::vector<Position> freeNeighbors;
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dy == 0 && dx == 0) continue;

                int cy = getY() + dy;
                int cx = getX() + dx;

                if (world.isCellFree(cy, cx)) {
                    freeNeighbors.push_back({cy, cx});
                }
            }
        }

        if (!freeNeighbors.empty()) {
            Position childPos = freeNeighbors[Random::getInt(0, static_cast<int>(freeNeighbors.size()) - 1)];
            world.addOffspring(getSpecies(), childPos.y, childPos.x);
            world.log("Reproduction: A new " + getSpecies() + " was born at (" 
                      + std::to_string(childPos.y) + ", " + std::to_string(childPos.x) + ").");
        }
        return;
    }

    // 2. Special interaction hook (e.g. Guarana bonus, toxic counter-effects)
    other.onAttackedBy(*this, world);

    // If attacker died during interaction, terminate combat immediately
    if (!isAlive()) {
        world.markForRemoval(this);
        return;
    }

    // 3. Combat resolution
    int attackerStrength = getStrength();
    int defenderStrength = other.getStrength();

    if (attackerStrength >= defenderStrength) {
        world.log("Combat: " + getSpecies() + " (" + std::to_string(attackerStrength) 
                  + ") defeated " + other.getSpecies() + " (" + std::to_string(defenderStrength) 
                  + ") at (" + std::to_string(other.getY()) + ", " + std::to_string(other.getX()) + ").");
        Position defenderPos = other.getPosition();
        other.die();
        world.markForRemoval(&other);
        world.moveOrganism(getPosition(), defenderPos);
    } else {
        handleCombatDefeat(other, attackerStrength, defenderStrength, world);
    }
}

void Animal::handleCombatDefeat(Organism& victor, int attackerStr, int defenderStr, World& world) {
    world.log("Combat: " + victor.getSpecies() + " (" + std::to_string(defenderStr) 
              + ") defended against " + getSpecies() + " (" + std::to_string(attackerStr) 
              + "). " + getSpecies() + " died.");
    die();
    world.markForRemoval(this);
}

bool Animal::findNewPosition(World& world, Position& targetPos) {
    const int range = getMovementRange();
    std::vector<Position> candidates;
    candidates.reserve((2 * range + 1) * (2 * range + 1) - 1);

    for (int dy = -range; dy <= range; ++dy) {
        for (int dx = -range; dx <= range; ++dx) {
            if (dy == 0 && dx == 0) {
                continue;
            }

            int candidateY = getY() + dy;
            int candidateX = getX() + dx;

            if (world.isWithinBounds(candidateY, candidateX)) {
                candidates.push_back({candidateY, candidateX});
            }
        }
    }

    if (candidates.empty()) {
        return false;
    }

    targetPos = candidates[Random::getInt(0, static_cast<int>(candidates.size()) - 1)];
    return true;
}
