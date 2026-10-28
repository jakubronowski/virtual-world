#include "Sheep.h"

Sheep::Sheep(Position pos)
    : Animal(pos, 4, 4, 2) {}

char Sheep::getSymbol() const {
    return 'S';
}

std::string Sheep::getSpecies() const {
    return "Sheep";
}

