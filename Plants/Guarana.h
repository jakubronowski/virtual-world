#ifndef GUARANA_H
#define GUARANA_H

#include "../Plant.h"

class Guarana : public Plant {
public:
    Guarana(Position pos);
    [[nodiscard]] char getSymbol() const override;
    [[nodiscard]] std::string getSpecies() const override;

    // Permanently increases the attacking animal's strength by +3
    void onAttackedBy(Organism& attacker, World& world) override;
};

#endif // GUARANA_H

