#include "Thorn.h"
#include "../World.h"

Thorn::Thorn(Position pos)
    : Plant(pos, 2, 19) {}

char Thorn::getSymbol() const {
    return 't';
}

std::string Thorn::getSpecies() const {
    return "Thorn";
}

void Thorn::onAttackedBy(Organism& attacker, World& world) {
    if (attacker.getStrength() < getStrength()) {
        attacker.die();
        world.log("Hazard: " + attacker.getSpecies() + " (" + std::to_string(attacker.getStrength()) 
                  + ") stepped on Thorn (strength 2) at (" + std::to_string(getY()) 
                  + ", " + std::to_string(getX()) + ") and was impaled to death!");
    }
}

