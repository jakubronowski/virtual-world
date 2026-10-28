#include "WorldRenderer.h"
#include "World.h"
#include "Organism.h"

#include <iostream>
#include <iomanip>

void WorldRenderer::render(const World& world, std::ostream& os) {
    os << "\n=====================================================================\n";
    os << " WORLD STATE AFTER TURN " << world.getTurnNumber() 
       << " (Active Organisms: " << world.getOrganismCount() << ")\n";
    os << "=====================================================================\n\n";

    // Column numbers
    os << "     ";
    for (int x = 0; x < World::WIDTH; ++x) {
        os << std::setw(5) << x;
    }
    os << "\n";

    // Grid rows
    for (int y = 0; y < World::HEIGHT; ++y) {
        os << std::setw(4) << y << " ";
        for (int x = 0; x < World::WIDTH; ++x) {
            const Organism* org = world.getOrganism(y, x);
            if (org != nullptr) {
                os << std::setw(5) << org->getSymbol();
            } else {
                os << std::setw(5) << '.';
            }
        }
        os << "\n";
    }

    // Turn events
    os << "\n--------------------------- TURN EVENTS ---------------------------\n";
    const auto& events = world.getTurnEvents();
    if (events.empty()) {
        os << "  (No notable events occurred this turn)\n";
    } else {
        for (const auto& event : events) {
            os << "  * " << event << "\n";
        }
    }
    os << "===================================================================\n";
}

void WorldRenderer::render(const World& world) {
    render(world, std::cout);
}
