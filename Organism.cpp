#include "Organism.h"
#include <iostream>

Organism::Organism(Position pos, int strength, int initiative, int id)
    : position(pos), strength(strength), initiative(initiative), id(id) {}

void Organism::draw() const {
    std::cout << getSymbol();
}

void Organism::onAttackedBy(Organism&, World&) {
    // Default: standard entity has no special interaction before combat
}

