#include <iostream>
#include <string>
#include <limits>
#include "World.h"
#include "Random.h"
#include "Exceptions.h"

namespace {
    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }

    void displayMenu() {
        std::cout << "\n============================== ACTION MENU ==============================\n";
        std::cout << " (n) Next Turn\n";
        std::cout << " (b) Run Batch (Simulate N turns)\n";
        std::cout << " (s) Save Simulation State\n";
        std::cout << " (w) Load Simulation State\n";
        std::cout << " (c) Clear Terminal Screen\n";
        std::cout << " (a) Spawn Organism\n";
        std::cout << " (r) Remove Organism\n";
        std::cout << " (p) Transfer Organism to Second World\n";
        std::cout << " (q) Quit Simulation\n";
        std::cout << "=========================================================================\n";
        std::cout << "Select option > ";
    }

    // Deterministic showcase scenario (Mosquito swarm synergy, Hare near Guarana, Wolf predator)
    void seedShowcaseScenario(World& world) {
        world.addOrganism("Fox", 5, 12);
        world.addOrganism("Grass", 19, 0);
        world.addOrganism("Mosquito", 3, 2);
        world.addOrganism("Mosquito", 3, 1);
        world.addOrganism("Mosquito", 3, 0);
        world.addOrganism("Guarana", 4, 4);
        world.addOrganism("Guarana", 4, 5);
        world.addOrganism("Thorn", 19, 19);
        world.addOrganism("Hare", 4, 7);
        world.addOrganism("Hare", 4, 8);
        world.addOrganism("Hare", 4, 2);
        world.addOrganism("Wolf", 5, 5);
        world.addOrganism("Sheep", 0, 2);
        world.addOrganism("Sheep", 0, 0);
        world.addOrganism("Mosquito", 3, 9);
        world.addOrganism("Mosquito", 3, 8);
        world.addOrganism("Mosquito", 3, 6);
    }

    void seedRandomSpecies(World& world, const std::string& species, int count) {
        int spawned = 0;
        int attempts = 0;
        constexpr int maxAttempts = 500;

        while (spawned < count && attempts < maxAttempts) {
            int y = Random::getInt(0, World::HEIGHT - 1);
            int x = Random::getInt(0, World::WIDTH - 1);
            if (world.isCellFree(y, x)) {
                world.addOrganism(species, y, x);
                ++spawned;
            }
            ++attempts;
        }
    }

    // Procedural random ecosystem distribution
    void seedRandomEcosystem(World& world) {
        seedRandomSpecies(world, "Wolf", 2);
        seedRandomSpecies(world, "Sheep", 4);
        seedRandomSpecies(world, "Fox", 2);
        seedRandomSpecies(world, "Hare", 3);
        seedRandomSpecies(world, "Mosquito", 6);
        seedRandomSpecies(world, "Grass", 4);
        seedRandomSpecies(world, "Guarana", 3);
        seedRandomSpecies(world, "Thorn", 2);
    }
}

int main() {
    std::cout << "=====================================================================\n";
    std::cout << "           VIRTUAL WORLD 2D SIMULATION                               \n";
    std::cout << "=====================================================================\n\n";

    World primaryWorld;
    World secondaryWorld;

    std::cout << "Select initial ecosystem setup:\n";
    std::cout << " (1) Showcase Scenario (Deterministic - Mosquito swarm, Guarana buff, Wolf predator)\n";
    std::cout << " (2) Procedural Random Ecosystem (Random distributed species)\n";
    std::cout << " (3) Empty World (Spawn manually or load state from file)\n";
    std::cout << "Select setup [1-3, default: 1] > ";

    char startupChoice = '1';
    if (std::cin >> startupChoice) {
        // clear remaining line
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    try {
        if (startupChoice == '2') {
            std::cout << "\nGenerating procedural random ecosystem...\n";
            seedRandomEcosystem(primaryWorld);
        } else if (startupChoice == '3') {
            std::cout << "\nInitialized empty simulation grid.\n";
        } else {
            std::cout << "\nLoading deterministic showcase scenario...\n";
            seedShowcaseScenario(primaryWorld);
        }
    } catch (const SimulationException& e) {
        std::cerr << "Initialization error: " << e.what() << "\n";
    }

    primaryWorld.render();

    char choice = ' ';
    do {
        displayMenu();
        if (!(std::cin >> choice)) {
            break;
        }

        try {
            switch (choice) {
                case 'n':
                    primaryWorld.executeTurn();
                    primaryWorld.render();
                    break;
                case 'b': {
                    int turnCount = 5;
                    std::cout << "Enter number of turns to simulate: ";
                    if (std::cin >> turnCount && turnCount > 0) {
                        for (int i = 0; i < turnCount; ++i) {
                            primaryWorld.executeTurn();
                        }
                        primaryWorld.render();
                    }
                    break;
                }
                case 's':
                    primaryWorld.saveToFile("world_state.txt");
                    std::cout << "State successfully saved to: world_state.txt\n";
                    break;
                case 'w':
                    primaryWorld.loadFromFile("world_state.txt");
                    std::cout << "State successfully loaded from: world_state.txt\n";
                    primaryWorld.render();
                    break;
                case 'c':
                    clearScreen();
                    primaryWorld.render();
                    break;
                case 'a': {
                    std::string species;
                    int y = 0, x = 0;
                    std::cout << "Enter species (Wolf, Sheep, Fox, Hare, Mosquito, Grass, Guarana, Thorn): ";
                    std::cin >> species;
                    std::cout << "Enter Row (Y coordinate, 0-" << (World::HEIGHT - 1) << "): ";
                    std::cin >> y;
                    std::cout << "Enter Column (X coordinate, 0-" << (World::WIDTH - 1) << "): ";
                    std::cin >> x;

                    primaryWorld.addOrganism(species, y, x);
                    std::cout << "Successfully spawned " << species << " at (" << y << ", " << x << ").\n";
                    primaryWorld.render();
                    break;
                }
                case 'r': {
                    int y = 0, x = 0;
                    std::cout << "Enter Row (Y coordinate) to remove: ";
                    std::cin >> y;
                    std::cout << "Enter Column (X coordinate) to remove: ";
                    std::cin >> x;

                    primaryWorld.removeOrganism(y, x);
                    std::cout << "Organism at (" << y << ", " << x << ") removed.\n";
                    primaryWorld.render();
                    break;
                }
                case 'p': {
                    int y = 0, x = 0, destY = 0, destX = 0;
                    std::cout << "Enter source coordinates (Y, X): ";
                    std::cin >> y >> x;
                    std::cout << "Enter destination coordinates in secondary world (Y, X): ";
                    std::cin >> destY >> destX;

                    Organism* target = primaryWorld.getOrganism(y, x);
                    if (!target) {
                        throw EntityNotFoundException(y, x);
                    }

                    primaryWorld.transferOrganism(target, secondaryWorld, destY, destX);
                    std::cout << "Organism transferred successfully!\n";
                    std::cout << "\n[Secondary World View]:\n";
                    secondaryWorld.render();
                    break;
                }
                case 'q':
                    std::cout << "Terminating simulation.\n";
                    break;
                default:
                    std::cout << "Unrecognized command: '" << choice << "'. Please select a valid option.\n";
                    break;
            }
        } catch (const SimulationException& e) {
            std::cerr << "\n[Simulation Error] " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "\n[System Error] " << e.what() << "\n";
        }

    } while (choice != 'q');

    return 0;
}
