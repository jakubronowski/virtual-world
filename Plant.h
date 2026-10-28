#ifndef PLANT_H
#define PLANT_H

#include "Organism.h"

class Plant : public Organism {
public:
    Plant(Position pos, int strength, int id = 0);

    void action(World& world) override;

    // Percent chance (0-100) to attempt seed spreading in a turn
    [[nodiscard]] virtual int getSpreadChancePercent() const noexcept { return 50; }
};

#endif // PLANT_H

