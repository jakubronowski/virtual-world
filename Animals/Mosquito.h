#ifndef MOSQUITO_H
#define MOSQUITO_H

#include "../Animal.h"

class Mosquito : public Animal {
    int swarmStrengthBoost{0};
    int swarmInitiativeBoost{0};

    void revertSwarmBoost() noexcept;
    void calculateSwarmBoost(World& world);

public:
    Mosquito(Position pos);
    [[nodiscard]] char getSymbol() const override;
    [[nodiscard]] std::string getSpecies() const override;

    void action(World& world) override;

protected:
    void handleCombatDefeat(Organism& victor, int attackerStr, int defenderStr, World& world) override;
};

#endif // MOSQUITO_H

