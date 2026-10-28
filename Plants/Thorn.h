#ifndef THORN_H
#define THORN_H

#include "../Plant.h"

class Thorn : public Plant {
public:
    Thorn(Position pos);
    [[nodiscard]] char getSymbol() const override;
    [[nodiscard]] std::string getSpecies() const override;

    // Thorn has 100% spread chance instead of default 50%
    [[nodiscard]] int getSpreadChancePercent() const noexcept override { return 100; }

    // Kills attackers weaker than Thorn's strength
    void onAttackedBy(Organism& attacker, World& world) override;
};

#endif // THORN_H

