#include "world_config.h"

void WorldConfig::setup(World * world)
{
    setupComponents(world);
    setupSystems(world);
}