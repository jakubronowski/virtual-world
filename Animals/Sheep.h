#ifndef SHEEP_H
#define SHEEP_H

#include "../Animal.h"

class Sheep : public Animal {
public:
    Sheep(Position pos);
    [[nodiscard]] char getSymbol() const override;
    [[nodiscard]] std::string getSpecies() const override;
};

#endif // SHEEP_H

