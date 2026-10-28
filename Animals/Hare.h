#ifndef HARE_H
#define HARE_H

#include "../Animal.h"

class Hare : public Animal {
public:
    Hare(Position pos);
    [[nodiscard]] char getSymbol() const override;
    [[nodiscard]] std::string getSpecies() const override;

    // Movement range increased to 2 cells
    [[nodiscard]] int getMovementRange() const noexcept override { return 2; }
};

#endif // HARE_H

