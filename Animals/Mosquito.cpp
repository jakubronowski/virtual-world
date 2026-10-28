#include "Mosquito.h"
#include "../World.h"
#include "../Random.h"

Mosquito::Mosquito(Position pos)
    : Animal(pos, 1, 1, 11) {}

char Mosquito::getSymbol() const {
    return 'M';
}

std::string Mosquito::getSpecies() const {
    return "Mosquito";
}

void Mosquito::revertSwarmBoost() noexcept {
    if (swarmStrengthBoost > 0 || swarmInitiativeBoost > 0) {
        modifyStrength(-swarmStrengthBoost);
        modifyInitiative(-swarmInitiativeBoost);
        swarmStrengthBoost = 0;
        swarmInitiativeBoost = 0;
    }
}

void Mosquito::calculateSwarmBoost(World& world) {
    revertSwarmBoost();
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dy == 0 && dx == 0) continue;

            int checkY = getY() + dy;
            int checkX = getX() + dx;

            if (world.isWithinBounds(checkY, checkX)) {
                Organism* neighbor = world.getOrganism(checkY, checkX);
                if (neighbor && neighbor->getSpecies() == "Mosquito") {
                    ++swarmStrengthBoost;
                    ++swarmInitiativeBoost;
                }
            }
        }
    }

    if (swarmStrengthBoost > 0) {
        modifyStrength(swarmStrengthBoost);
        modifyInitiative(swarmInitiativeBoost);
    }
}

void Mosquito::action(World& world) {
    calculateSwarmBoost(world);
    Animal::action(world);
    revertSwarmBoost();
}

void Mosquito::handleCombatDefeat(Organism& victor, int attackerStr, int defenderStr, World& world) {
    if (Random::getInt(0, 99) < 50) {
        world.log("Combat: Mosquito was overpowered by " + victor.getSpecies() 
                  + " but dodged the fatal blow (50% evasion) and retreated!");
    } else {
        Animal::handleCombatDefeat(victor, attackerStr, defenderStr, world);
    }
}
