#include "World.h"
#include "Organism.h"
#include "Exceptions.h"

#include "OrganismFactory.h"
#include "WorldRenderer.h"

#include <algorithm>
#include <fstream>

World::World() {
    for (auto& row : grid) {
        row.fill(nullptr);
    }
}

World::~World() = default;
World::World(World&&) noexcept = default;
World& World::operator=(World&&) noexcept = default;

void World::render() const {
    WorldRenderer::render(*this);
}

void World::sortTurnQueue() {
    // std::stable_sort preserves relative order for equal initiative and age
    std::stable_sort(organisms.begin(), organisms.end(),
        [](const std::unique_ptr<Organism>& a, const std::unique_ptr<Organism>& b) {
            if (a->getInitiative() == b->getInitiative()) {
                return a->getAge() > b->getAge(); // Older organisms take turn first
            }
            return a->getInitiative() > b->getInitiative(); // Higher initiative first
        });
}

void World::executeTurn() {
    turnEvents.clear();
    ++turnNumber;

    sortTurnQueue();

    for (const auto& organism : organisms) {
        if (!organism->isAlive()) {
            continue; // Skip entities that died earlier in this turn
        }

        organism->incrementAge();
        organism->action(*this);
    }

    processPendingOperations();
}

void World::processPendingOperations() {
    // 1. Process deferred deletions
    for (Organism* toDelete : pendingDeletions) {
        if (toDelete) {
            Position pos = toDelete->getPosition();
            if (isWithinBounds(pos.y, pos.x) && grid[pos.y][pos.x] == toDelete) {
                grid[pos.y][pos.x] = nullptr;
            }
        }
    }
    std::erase_if(organisms, [this](const std::unique_ptr<Organism>& ptr) {
        return !ptr->isAlive() ||
               std::find(pendingDeletions.begin(), pendingDeletions.end(), ptr.get()) != pendingDeletions.end();
    });
    pendingDeletions.clear();

    // 2. Process deferred additions (offspring)
    for (auto& newborn : pendingAdditions) {
        if (newborn && newborn->isAlive()) {
            Position pos = newborn->getPosition();
            grid[pos.y][pos.x] = newborn.get();
            organisms.push_back(std::move(newborn));
        }
    }
    pendingAdditions.clear();
}

void World::addOrganism(const std::string& species, int y, int x) {
    if (!isWithinBounds(y, x)) {
        throw OutOfBoundsException(y, x);
    }
    if (grid[y][x] != nullptr) {
        throw CellOccupiedException(y, x);
    }

    auto organism = OrganismFactory::create(species, {y, x});
    if (!organism) {
        throw UnknownSpeciesException(species);
    }

    grid[y][x] = organism.get();
    organisms.push_back(std::move(organism));
}

void World::addOffspring(const std::string& species, int y, int x) {
    if (!isCellFree(y, x)) {
        return;
    }

    auto offspring = OrganismFactory::create(species, {y, x});
    if (offspring) {
        grid[y][x] = offspring.get(); // Reserve spot immediately
        pendingAdditions.push_back(std::move(offspring));
    }
}

void World::removeOrganism(int y, int x) {
    if (!isWithinBounds(y, x)) {
        throw OutOfBoundsException(y, x);
    }
    Organism* target = grid[y][x];
    if (!target) {
        throw EntityNotFoundException(y, x);
    }
    removeOrganism(target);
}

void World::removeOrganism(Organism* organism) {
    if (!organism) return;

    Position pos = organism->getPosition();
    if (isWithinBounds(pos.y, pos.x) && grid[pos.y][pos.x] == organism) {
        grid[pos.y][pos.x] = nullptr;
    }

    std::erase_if(organisms, [organism](const std::unique_ptr<Organism>& ptr) {
        return ptr.get() == organism;
    });
}

void World::markForRemoval(Organism* organism) {
    if (!organism) return;

    Position pos = organism->getPosition();
    if (isWithinBounds(pos.y, pos.x) && grid[pos.y][pos.x] == organism) {
        grid[pos.y][pos.x] = nullptr;
    }

    if (std::find(pendingDeletions.begin(), pendingDeletions.end(), organism) == pendingDeletions.end()) {
        pendingDeletions.push_back(organism);
    }
}

void World::moveOrganism(Position from, Position to) {
    if (!isWithinBounds(from.y, from.x) || !isWithinBounds(to.y, to.x)) {
        return;
    }

    Organism* entity = grid[from.y][from.x];
    grid[from.y][from.x] = nullptr;
    grid[to.y][to.x] = entity;

    if (entity) {
        entity->setPosition(to);
    }
}

Organism* World::getOrganism(int y, int x) const {
    if (isWithinBounds(y, x)) {
        return grid[y][x];
    }
    return nullptr;
}

bool World::isCellFree(int y, int x) const noexcept {
    return isWithinBounds(y, x) && grid[y][x] == nullptr;
}

bool World::isWithinBounds(int y, int x) const noexcept {
    return (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH);
}

void World::transferOrganism(Organism* organism, World& destinationWorld, int destY, int destX) {
    if (!organism) return;
    if (!destinationWorld.isWithinBounds(destY, destX)) {
        throw OutOfBoundsException(destY, destX);
    }
    if (!destinationWorld.isCellFree(destY, destX)) {
        throw CellOccupiedException(destY, destX);
    }

    Position currentPos = organism->getPosition();

    auto it = std::find_if(organisms.begin(), organisms.end(),
        [organism](const std::unique_ptr<Organism>& ptr) { return ptr.get() == organism; });

    if (it != organisms.end()) {
        std::unique_ptr<Organism> entity = std::move(*it);
        organisms.erase(it);
        grid[currentPos.y][currentPos.x] = nullptr;

        entity->setPosition({destY, destX});
        destinationWorld.grid[destY][destX] = entity.get();
        destinationWorld.organisms.push_back(std::move(entity));
    }
}

void World::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw SimulationException("Failed to open file for writing: " + filename);
    }

    file << turnNumber << "\n";

    for (const auto& org : organisms) {
        if (org && org->isAlive()) {
            file << org->getSpecies() << " "
                 << org->getAge() << " "
                 << org->getId() << " "
                 << org->getInitiative() << " "
                 << org->getStrength() << " "
                 << org->getY() << " "
                 << org->getX() << "\n";
        }
    }
}

void World::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw SimulationException("Failed to open file for reading: " + filename);
    }

    // Deterministic RAII cleanup
    organisms.clear();
    pendingAdditions.clear();
    pendingDeletions.clear();
    for (auto& row : grid) {
        row.fill(nullptr);
    }

    file >> turnNumber;

    std::string species;
    int age, id, initiative, strength, y, x;
    while (file >> species >> age >> id >> initiative >> strength >> y >> x) {
        try {
            addOrganism(species, y, x);
            Organism* loaded = getOrganism(y, x);
            if (loaded) {
                loaded->setAge(age);
                loaded->setId(id);
                loaded->setInitiative(initiative);
                loaded->setStrength(strength);
            }
        } catch (const std::exception& e) {
            log("Warning during load: " + std::string(e.what()));
        }
    }
}

void World::log(std::string event) {
    turnEvents.push_back(std::move(event));
}

