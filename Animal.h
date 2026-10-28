#ifndef ANIMAL_H
#define ANIMAL_H

#include "Organism.h"

class Animal : public Organism {
public:
    Animal(Position pos, int strength, int initiative, int id = 0);

    void action(World& world) override;
    virtual void collideWith(Organism& other, World& world);
    [[nodiscard]] virtual bool findNewPosition(World& world, Position& targetPos);

    // Virtual parameterization to allow species (like Hare) to customize movement step
    [[nodiscard]] virtual int getMovementRange() const noexcept { return 1; }

protected:
    // Hook for species-specific combat defeat behavior (e.g. Mosquito's evasion)
    virtual void handleCombatDefeat(Organism& victor, int attackerStr, int defenderStr, World& world);
};

#endif // ANIMAL_H

