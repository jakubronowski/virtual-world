#include "Grass.h"

Grass::Grass(Position pos)
    : Plant(pos, 0, 14) {}

char Grass::getSymbol() const {
    return 'g';
}

std::string Grass::getSpecies() const {
    return "Grass";
}

