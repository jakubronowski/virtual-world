#include "Guarana.h"
#include "../World.h"

Guarana::Guarana(Position pos)
    : Plant(pos, 0, 17) {}

char Guarana::getSymbol() const {
    return 'u';
}

std::string Guarana::getSpecies() const {
    return "Guarana";
}

void Guarana::onAttackedBy(Organism& attacker, World& world) {
    int oldStrength = attacker.getStrength();
    attacker.setStrength(oldStrength + 3);
    world.log("Bonus: " + attacker.getSpecies() + " consumed Guarana at (" 
              + std::to_string(getY()) + ", " + std::to_string(getX()) 
              + ")! Strength permanently increased from " + std::to_string(oldStrength) 
              + " to " + std::to_string(attacker.getStrength()) + " (+3).");
}
