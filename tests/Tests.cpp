#include <gtest/gtest.h>

#include "World.h"
#include "Organism.h"
#include "Animal.h"
#include "OrganismFactory.h"
#include "WorldRenderer.h"
#include "Exceptions.h"
#include <sstream>

// ==============================================================================
// 1. Factory & Polymorphism Tests
// ==============================================================================
TEST(FactoryTest, CreatesValidOrganismsWithCorrectAttributes) {
    auto wolf = OrganismFactory::create("Wolf", {2, 3});
    ASSERT_NE(wolf, nullptr);
    EXPECT_EQ(wolf->getSpecies(), "Wolf");
    EXPECT_EQ(wolf->getSymbol(), 'W');
    EXPECT_EQ(wolf->getStrength(), 9);
    EXPECT_EQ(wolf->getInitiative(), 5);
    EXPECT_EQ(wolf->getY(), 2);
    EXPECT_EQ(wolf->getX(), 3);

    auto sheep = OrganismFactory::create("Sheep", {0, 0});
    ASSERT_NE(sheep, nullptr);
    EXPECT_EQ(sheep->getSpecies(), "Sheep");
    EXPECT_EQ(sheep->getSymbol(), 'S');
    EXPECT_EQ(sheep->getStrength(), 4);
    EXPECT_EQ(sheep->getInitiative(), 4);

    auto grass = OrganismFactory::create("Grass", {5, 5});
    ASSERT_NE(grass, nullptr);
    EXPECT_EQ(grass->getSpecies(), "Grass");
    EXPECT_EQ(grass->getSymbol(), 'g');
    EXPECT_EQ(grass->getStrength(), 0);
    EXPECT_EQ(grass->getInitiative(), 0);
}

TEST(FactoryTest, ReturnsNullptrOnUnknownSpecies) {
    auto unknown = OrganismFactory::create("Dragon", {0, 0});
    EXPECT_EQ(unknown, nullptr);
}

// ==============================================================================
// 2. Grid & Domain Exception Tests
// ==============================================================================
TEST(WorldGridTest, BoundaryValidation) {
    World world;
    EXPECT_TRUE(world.isWithinBounds(0, 0));
    EXPECT_TRUE(world.isWithinBounds(World::HEIGHT - 1, World::WIDTH - 1));
    EXPECT_FALSE(world.isWithinBounds(-1, 0));
    EXPECT_FALSE(world.isWithinBounds(0, -1));
    EXPECT_FALSE(world.isWithinBounds(World::HEIGHT, 0));
    EXPECT_FALSE(world.isWithinBounds(0, World::WIDTH));
}

TEST(WorldGridTest, ThrowsOutOfBoundsException) {
    World world;
    EXPECT_THROW(world.addOrganism("Wolf", -1, 5), OutOfBoundsException);
    EXPECT_THROW(world.addOrganism("Wolf", 25, 5), OutOfBoundsException);
}

TEST(WorldGridTest, ThrowsCellOccupiedException) {
    World world;
    world.addOrganism("Wolf", 5, 5);
    EXPECT_FALSE(world.isCellFree(5, 5));
    EXPECT_THROW(world.addOrganism("Sheep", 5, 5), CellOccupiedException);
}

TEST(WorldGridTest, ThrowsUnknownSpeciesException) {
    World world;
    EXPECT_THROW(world.addOrganism("Unicorn", 3, 3), UnknownSpeciesException);
}

// ==============================================================================
// 3. Combat Mechanics Tests
// ==============================================================================
TEST(CombatTest, StrongerAttackerDefeatsWeakerDefender) {
    World world;
    world.addOrganism("Wolf", 5, 5);  // Strength: 9
    world.addOrganism("Sheep", 5, 6); // Strength: 4

    auto* wolf = dynamic_cast<Animal*>(world.getOrganism(5, 5));
    auto* sheep = world.getOrganism(5, 6);
    ASSERT_NE(wolf, nullptr);
    ASSERT_NE(sheep, nullptr);

    wolf->collideWith(*sheep, world);

    // Wolf should survive, take the sheep's cell; sheep should die
    EXPECT_TRUE(wolf->isAlive());
    EXPECT_FALSE(sheep->isAlive());
    EXPECT_EQ(wolf->getY(), 5);
    EXPECT_EQ(wolf->getX(), 6);
    EXPECT_EQ(world.getOrganism(5, 6), wolf);
}

TEST(CombatTest, EqualStrengthAttackerWins) {
    World world;
    // Fox has base strength 3. Hare has base strength 2, boosted to 3 for equal match-up:
    world.addOrganism("Fox", 8, 8);  // Attacker: Strength 3
    world.addOrganism("Hare", 8, 9); // Defender: Strength 2 -> adjusted to 3
    auto* hare = world.getOrganism(8, 9);
    ASSERT_NE(hare, nullptr);
    hare->setStrength(3);

    auto* fox = dynamic_cast<Animal*>(world.getOrganism(8, 8));
    ASSERT_NE(fox, nullptr);
    fox->collideWith(*hare, world);

    // On equal strength (3 vs 3), attacker (Fox) wins and occupies target cell
    EXPECT_TRUE(fox->isAlive());
    EXPECT_FALSE(hare->isAlive());
    EXPECT_EQ(world.getOrganism(8, 9), fox);
}

// ==============================================================================
// 4. Plant Interaction Hooks
// ==============================================================================
TEST(PlantTest, GuaranaPermanentlyIncreasesAttackerStrength) {
    World world;
    world.addOrganism("Sheep", 4, 4);   // Base strength: 4
    world.addOrganism("Guarana", 4, 5); // Consumable plant (+3 buff)

    auto* sheep = dynamic_cast<Animal*>(world.getOrganism(4, 4));
    auto* guarana = world.getOrganism(4, 5);
    ASSERT_NE(sheep, nullptr);
    ASSERT_NE(guarana, nullptr);

    sheep->collideWith(*guarana, world);

    // Strength should be permanently increased by +3 (4 -> 7)
    EXPECT_EQ(sheep->getStrength(), 7);
    EXPECT_TRUE(sheep->isAlive());
    EXPECT_FALSE(guarana->isAlive());
}

TEST(PlantTest, ThornKillsWeakerAttacker) {
    World world;
    world.addOrganism("Mosquito", 10, 10); // Strength: 1 (< Thorn's strength 2)
    world.addOrganism("Thorn", 10, 11);    // Strength: 2

    auto* mosquito = dynamic_cast<Animal*>(world.getOrganism(10, 10));
    auto* thorn = world.getOrganism(10, 11);
    ASSERT_NE(mosquito, nullptr);
    ASSERT_NE(thorn, nullptr);

    mosquito->collideWith(*thorn, world);

    // Mosquito should perish on thorn
    EXPECT_FALSE(mosquito->isAlive());
    EXPECT_TRUE(thorn->isAlive());
}

// ==============================================================================
// 5. Organism Ownership Transfer (Move Semantics)
// ==============================================================================
TEST(TransferTest, MovesOrganismOwnershipBetweenWorlds) {
    World worldA;
    World worldB;

    worldA.addOrganism("Fox", 3, 3);
    EXPECT_EQ(worldA.getOrganismCount(), 1);
    EXPECT_EQ(worldB.getOrganismCount(), 0);

    auto* fox = worldA.getOrganism(3, 3);
    ASSERT_NE(fox, nullptr);

    worldA.transferOrganism(fox, worldB, 7, 7);

    // Source world spot should be freed and count decremented
    EXPECT_TRUE(worldA.isCellFree(3, 3));
    EXPECT_EQ(worldA.getOrganismCount(), 0);

    // Destination world spot should be occupied by the transferred entity
    EXPECT_FALSE(worldB.isCellFree(7, 7));
    EXPECT_EQ(worldB.getOrganismCount(), 1);
    EXPECT_EQ(worldB.getOrganism(7, 7)->getSpecies(), "Fox");
}

TEST(TransferTest, ThrowsOnInvalidTransferDestination) {
    World worldA;
    World worldB;

    worldA.addOrganism("Wolf", 1, 1);
    worldB.addOrganism("Sheep", 2, 2);

    auto* wolf = worldA.getOrganism(1, 1);

    // Destination cell occupied -> CellOccupiedException
    EXPECT_THROW(worldA.transferOrganism(wolf, worldB, 2, 2), CellOccupiedException);

    // Destination out of bounds -> OutOfBoundsException
    EXPECT_THROW(worldA.transferOrganism(wolf, worldB, -5, 100), OutOfBoundsException);
}

// ==============================================================================
// 6. Turn Lifecycle & Deferred Cleanups
// ==============================================================================
TEST(LifecycleTest, DeferredCleanupsPurgeDeadOrganisms) {
    World world;
    // Place organisms far apart so survivor's movement cannot step into the dead entity's cell
    world.addOrganism("Wolf", 10, 10);
    world.addOrganism("Sheep", 1, 2);
    EXPECT_EQ(world.getOrganismCount(), 2);

    // Kill sheep manually via markForRemoval
    auto* sheep = world.getOrganism(1, 2);
    sheep->die();
    world.markForRemoval(sheep);

    // Before executeTurn, container still has 2 unique_ptrs
    EXPECT_EQ(world.getOrganismCount(), 2);

    // After executeTurn, deferred deletion should purge the dead entity
    world.executeTurn();
    EXPECT_EQ(world.getOrganismCount(), 1);
    EXPECT_TRUE(world.isCellFree(1, 2));
}

// ==============================================================================
// 7. Renderer Stream Injection Test
// ==============================================================================
TEST(RendererTest, OutputsWorldStateToStringStream) {
    World world;
    world.addOrganism("Wolf", 0, 0);

    std::ostringstream oss;
    WorldRenderer::render(world, oss);
    std::string output = oss.str();

    EXPECT_NE(output.find("WORLD STATE AFTER TURN 1"), std::string::npos);
    EXPECT_NE(output.find("Active Organisms: 1"), std::string::npos);
    EXPECT_NE(output.find('W'), std::string::npos);
}

