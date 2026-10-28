#include "OrganismFactory.h"
#include "Organism.h"

#include "Animals/Wolf.h"
#include "Animals/Sheep.h"
#include "Animals/Fox.h"
#include "Animals/Hare.h"
#include "Animals/Mosquito.h"
#include "Plants/Grass.h"
#include "Plants/Guarana.h"
#include "Plants/Thorn.h"

std::unique_ptr<Organism> OrganismFactory::create(const std::string& species, Position pos) {
    if (species == "Wolf")  return std::make_unique<Wolf>(pos);
    if (species == "Sheep")  return std::make_unique<Sheep>(pos);
    if (species == "Fox")   return std::make_unique<Fox>(pos);
    if (species == "Hare") return std::make_unique<Hare>(pos);
    if (species == "Mosquito") return std::make_unique<Mosquito>(pos);
    if (species == "Grass") return std::make_unique<Grass>(pos);
    if (species == "Guarana") return std::make_unique<Guarana>(pos);
    if (species == "Thorn") return std::make_unique<Thorn>(pos);
    return nullptr;
}

