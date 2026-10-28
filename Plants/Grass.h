#ifndef GRASS_H
#define GRASS_H

#include "../Plant.h"

class Grass : public Plant {
public:
    Grass(Position pos);
    [[nodiscard]] char getSymbol() const override;
    [[nodiscard]] std::string getSpecies() const override;
};

#endif // GRASS_H

