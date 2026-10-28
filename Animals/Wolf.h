#ifndef WOLF_H
#define WOLF_H

#include "../Animal.h"

class Wolf : public Animal {
public:
    Wolf(Position pos);
    [[nodiscard]] char getSymbol() const override;
    [[nodiscard]] std::string getSpecies() const override;
};

#endif // WOLF_H

