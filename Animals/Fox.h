#ifndef FOX_H
#define FOX_H

#include "../Animal.h"

class Fox : public Animal {
public:
    Fox(Position pos);
    [[nodiscard]] char getSymbol() const override;
    [[nodiscard]] std::string getSpecies() const override;

    // Fox's keen sense of smell prevents it from moving onto cells with stronger opponents
    [[nodiscard]] bool findNewPosition(World& world, Position& targetPos) override;
};

#endif // FOX_H

