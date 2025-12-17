#pragma once

#include "core/world_config.h"
#include "core/texture_loader.h"


#include "system/window_system.h"
#include "system/render_system.h"
#include "system/player_controller_system.h"
#include "system/movement_system.h"
#include "system/animation_system.h"
#include "system/collider_system.h"


class DefaultWorldConfig : public WorldConfig
{
public:
    void setupComponents(World *world) override;
    void setupSystems(World *world) override;

    void update(float deltaTime);

private:
    std::shared_ptr<WindowSystem> windowSystem;
    std::shared_ptr<RenderSystem> renderSystem;
    std::shared_ptr<PlayerControllerSystem> playerControllerSystem;
    std::shared_ptr<MovementSystem> movementSystem;
    std::shared_ptr<AnimationSystem> animationSystem;
    std::shared_ptr<ColliderSystem> colliderSystem;

    void processSDLEvents();
};