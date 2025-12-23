#include "test_world_builder.h"

#include "components.h"

void TestWorldBuilder::buildWorld(World *world)
{

    buildPlayer(world);
    buildEnvironment(world);

    std::cerr << "Game Engine initialized." << std::endl;
}

void TestWorldBuilder::buildEnvironment(World *world)
{
    SDL_Texture *tileTexture = world->getTexture("F:\\Users\\User\\Documents\\game_studio\\projects\\ecs\\assets\\arts\\gfx\\Overworld.png");

    glm::ivec2 grass = glm::ivec2(0, 0);
    glm::ivec2 grassWaveCornerBottomLeft = glm::ivec2(3, 0);
    glm::ivec2 grassWaveBottom = glm::ivec2(3, 1);
    glm::ivec2 grassWaveCornerBottomRight = glm::ivec2(3, 2);
    glm::ivec2 grassWaveRight = glm::ivec2(4, 0);
    glm::ivec2 wave = glm::ivec2(4, 1);
    glm::ivec2 grassWaveLeft = glm::ivec2(4, 2);
    glm::ivec2 grassWaveCornerTopLeft = glm::ivec2(5, 0);
    glm::ivec2 grassWaveTop = glm::ivec2(5, 1);
    glm::ivec2 grassWaveCornerTopRight = glm::ivec2(5, 2);
    
    glm::ivec2 stoneWallGrass = glm::ivec2(5,11);
    glm::ivec2 stoneWallTop = glm::ivec2(5,12);
    glm::ivec2 stoneWallMid = glm::ivec2(5,13);
    glm::ivec2 stoneWallBottom = glm::ivec2(5, 14);

    float tileSize = 16.0f;

    auto placeTile = [&](glm::ivec2 srcPosition, int x, int y)
    {
        Entity entity = world->createEntity();
        RenderComponent renderComponent;
        renderComponent.texture = tileTexture;
        renderComponent.srcPosition = srcPosition * 16;
        renderComponent.srcSize = glm::vec2(tileSize, tileSize);
        renderComponent.destPosition = glm::vec2(0.0f, 0.0f);
        renderComponent.destSize = glm::vec2(tileSize, tileSize);
        world->addComponent<RenderComponent>(entity, renderComponent);

        Transform transform;
        transform.position = glm::vec3(static_cast<float>(x), static_cast<float>(y), -1.0f);
        transform.rotation = 0.0f;
        transform.scale = glm::vec2(1.0f, 1.0f);
        world->addComponent<Transform>(entity, transform);
    };

    for (int x = 0; x < 48; ++x)
    {
        placeTile(stoneWallGrass, x * tileSize, 0 * tileSize);
    }

       for (int x = 0; x < 48; ++x)
    {
        placeTile(stoneWallTop, x * tileSize, 1 * tileSize);
    }

       for (int x = 0; x < 48; ++x)
    {
        placeTile(stoneWallMid, x * tileSize, 2 * tileSize);
    }   

       for (int x = 0; x < 48; ++x)
    {
        placeTile(stoneWallBottom, x * tileSize, 3 * tileSize);
    }
    for (int y = 4; y < 27; ++y)
    {
        for (int x = 0; x < 48; ++x)
        {
            placeTile(grass, x * tileSize, y * tileSize);
        }
    }


    auto entity =  world->createEntity();
    Transform transform;
    transform.position = glm::vec3(200.0f, 200.0f, 0.0f);
    transform.rotation = 0.0f;
    transform.scale = glm::vec2(1.0f, 1.0f);
    world->addComponent<Transform>(entity, transform);
    ColliderComponent collider;
    collider.shape = ColliderComponent::Shape::Rectangle;
    collider.isStatic = true;
    collider.offset = glm::vec2(0.0f, 0.0f);
    collider.size = glm::vec2(32.0f, 32.0f);
    world->addComponent<ColliderComponent>(entity, collider);
    RenderComponent renderComponent;
    renderComponent.texture = tileTexture;
    renderComponent.srcPosition = glm::vec2(5*16.0f, 16*16.0f);
    renderComponent.srcSize = glm::vec2(32.0f, 32.0f);
    renderComponent.destPosition = glm::vec2(0.0f, 0.0f);
    renderComponent.destSize = glm::vec2(32.0f, 32.0f);
    world->addComponent<RenderComponent>(entity, renderComponent);

}

void TestWorldBuilder::buildPlayer(World *world)
{
    SDL_Texture *charTexture = world->getTexture("F:\\Users\\User\\Documents\\game_studio\\projects\\ecs\\assets\\arts\\gfx\\character.png");
    Entity entity = world->createEntity();
    RenderComponent renderComponent;
    renderComponent.texture = charTexture;
    renderComponent.srcPosition = glm::vec2(0.0f, 0.0f);
    renderComponent.srcSize = glm::vec2(16.0f, 32.0f);
    renderComponent.destPosition = glm::vec2(0.0f, 0.0f);
    renderComponent.destSize = glm::vec2(16.0f, 32.0f);
    world->addComponent<RenderComponent>(entity, renderComponent);

    AnimationComponent animationComponent;
    animationComponent.playing = true;
    animationComponent.loop = true;
    animationComponent.currentFrameIndex = 0;
    animationComponent.elapsedTime = 0.0f;

    AnimationControllerComponent animController;
    animController.stateAnimations["walk_down"] =
        {
            // Add frames here
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(16.0f, 0.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(32.0f, 0.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(48.0f, 0.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
        };

    animController.stateAnimations["walk_up"] =
        {
            // Add frames here
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(0.0f, 64.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(16.0f, 64.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(32.0f, 64.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(48.0f, 64.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
        };

    animController.stateAnimations["walk_right"] =
        {
            // Add frames here
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(0.0f, 32.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(16.0f, 32.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(32.0f, 32.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(48.0f, 32.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
        };

    animController.stateAnimations["walk_left"] =
        {
            // Add frames here
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(0.0f, 96.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(16.0f, 96.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(32.0f, 96.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
            AnimationFrame{
                0.1f,
                charTexture,
                glm::vec2(48.0f, 96.0f),
                glm::vec2(16.0f, 32.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(16.0f, 32.0f)},
        };

    world->addComponent<AnimationComponent>(entity, animationComponent);
    world->addComponent<AnimationControllerComponent>(entity, animController);

    Transform transform;
    transform.position = glm::vec3(100.0f, 100.0f, 0.0f);
    transform.rotation = 0.0f;
    transform.scale = glm::vec2(1.0f, 1.0f);
    world->addComponent<Transform>(entity, transform);

    PlayerControlComponent playerControlComponent;
    playerControlComponent.speed = 200.0f;
    world->addComponent<PlayerControlComponent>(entity, playerControlComponent);

    VelocityComponent velocityComponent;
    velocityComponent.velocity = glm::vec2(0.0f, 0.0f);
    world->addComponent<VelocityComponent>(entity, velocityComponent);

    ColliderComponent collider;
    collider.shape = ColliderComponent::Shape::Rectangle;
    collider.isStatic = false;
    collider.offset = glm::vec2(0.0f, 0.0f);
    collider.size = glm::vec2(16.0f, 32.0f);
    world->addComponent<ColliderComponent>(entity, collider);
    
}
