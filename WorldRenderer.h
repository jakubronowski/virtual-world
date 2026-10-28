#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include <iosfwd>

class World;

class WorldRenderer {
public:
    static void render(const World& world, std::ostream& os);
    static void render(const World& world);
};

#endif // WORLD_RENDERER_H
