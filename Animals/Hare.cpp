#include "Hare.h"

Hare::Hare(Position pos)
    : Animal(pos, 2, 2, 13) {}

char Hare::getSymbol() const {
    return 'H';
}

std::string Hare::getSpecies() const {
    return "Hare";
}

