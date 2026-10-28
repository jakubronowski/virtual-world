#ifndef ORGANISM_FACTORY_H
#define ORGANISM_FACTORY_H

#include <memory>
#include <string>
#include "Position.h"

class Organism;

class OrganismFactory {
public:
    [[nodiscard]] static std::unique_ptr<Organism> create(const std::string& species, Position pos);
};

#endif // ORGANISM_FACTORY_H

