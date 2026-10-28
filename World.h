#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <array>
#include <memory>
#include <string>

#include "Position.h"

class Organism;

class World {
public:
    static constexpr int WIDTH = 20;
    static constexpr int HEIGHT = 20;

private:
    int turnNumber{1};

    // Single ownership of all active simulation entities
    std::vector<std::unique_ptr<Organism>> organisms;
    std::vector<std::unique_ptr<Organism>> pendingAdditions;
    std::vector<Organism*> pendingDeletions;

    // Contiguous 2D non-owning observer grid
    std::array<std::array<Organism*, WIDTH>, HEIGHT> grid{};

    // Turn event log
    std::vector<std::string> turnEvents;

    void sortTurnQueue();
    void processPendingOperations();

public:
    World();
    ~World();

    // Non-copyable
    World(const World&) = delete;
    World& operator=(const World&) = delete;

    // Movable
    World(World&&) noexcept;
    World& operator=(World&&) noexcept;

    void render() const;
    void executeTurn();

    void addOrganism(const std::string& species, int y, int x);
    void addOffspring(const std::string& species, int y, int x);
    void removeOrganism(int y, int x);
    void removeOrganism(Organism* organism);
    void markForRemoval(Organism* organism);
    void transferOrganism(Organism* organism, World& destinationWorld, int destY, int destX);

    [[nodiscard]] Organism* getOrganism(int y, int x) const;
    void moveOrganism(Position from, Position to);

    [[nodiscard]] bool isCellFree(int y, int x) const noexcept;
    [[nodiscard]] bool isWithinBounds(int y, int x) const noexcept;

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    void log(std::string event);
    [[nodiscard]] const std::vector<std::string>& getTurnEvents() const noexcept { return turnEvents; }

    [[nodiscard]] int getTurnNumber() const noexcept { return turnNumber; }
    [[nodiscard]] size_t getOrganismCount() const noexcept { return organisms.size(); }
};

#endif // WORLD_H
