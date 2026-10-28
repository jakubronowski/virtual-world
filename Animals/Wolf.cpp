#include "Wolf.h"

Wolf::Wolf(Position pos)
    : Animal(pos, 9, 5, 1) {}

char Wolf::getSymbol() const {
    return 'W';
}

std::string Wolf::getSpecies() const {
    return "Wolf";
}

