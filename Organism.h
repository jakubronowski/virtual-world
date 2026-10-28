#ifndef ORGANISM_H
#define ORGANISM_H

#include "Position.h"
#include <string>

class World;

class Organism {
private:
    Position position;
    int strength;
    int initiative;
    int age{0};
    bool alive{true};
    int id{0};

protected:
    void modifyStrength(int delta) noexcept { strength += delta; }
    void modifyInitiative(int delta) noexcept { initiative += delta; }

public:
    Organism(Position pos, int strength, int initiative, int id = 0);
    virtual ~Organism() = default;

    // Pure virtual interface
    virtual void action(World& world) = 0;
    virtual void draw() const;
    [[nodiscard]] virtual char getSymbol() const = 0;
    [[nodiscard]] virtual std::string getSpecies() const = 0;

    // Polymorphic interaction hook: called when an attacker steps onto this organism's cell
    virtual void onAttackedBy(Organism& attacker, World& world);

    // Getters
    [[nodiscard]] Position getPosition() const noexcept { return position; }
    [[nodiscard]] int getY() const noexcept { return position.y; }
    [[nodiscard]] int getX() const noexcept { return position.x; }
    [[nodiscard]] int getStrength() const noexcept { return strength; }
    [[nodiscard]] int getInitiative() const noexcept { return initiative; }
    [[nodiscard]] int getAge() const noexcept { return age; }
    [[nodiscard]] bool isAlive() const noexcept { return alive; }
    [[nodiscard]] int getId() const noexcept { return id; }

    // Setters
    void setPosition(Position newPos) noexcept { position = newPos; }
    void setPosition(int newY, int newX) noexcept { position = {newY, newX}; }
    void setStrength(int newStrength) noexcept { strength = newStrength; }
    void setInitiative(int newInitiative) noexcept { initiative = newInitiative; }
    void setAge(int newAge) noexcept { age = newAge; }
    void incrementAge() noexcept { ++age; }
    void setId(int newId) noexcept { id = newId; }
    void die() noexcept { alive = false; }
};

#endif // ORGANISM_H

