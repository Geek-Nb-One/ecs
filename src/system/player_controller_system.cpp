#include "player_controller_system.h"

void PlayerControllerSystem::processEvents(SDL_Event &sdlEvent)
{

    if (sdlEvent.type == SDL_EVENT_GAMEPAD_ADDED)
    {
        if (!gamepad)
        {
            connectGamePad(sdlEvent.jdevice.which);
        }
    }

    else if (sdlEvent.type == SDL_EVENT_GAMEPAD_REMOVED)
    {
        if (gamepad && SDL_GetGamepadID(gamepad) == sdlEvent.jdevice.which)
        {
            SDL_CloseGamepad(gamepad);
            gamepad = nullptr;
            std::cout << "Gamepad disconnected." << std::endl;
            scanGamepad();
        }
    }
    // Process SDL events related to player control
    else if (sdlEvent.type == SDL_EVENT_KEY_DOWN)
    {
        // Handle key down events
        switch (sdlEvent.key.key)
        {
        case SDLK_W:
            // Move player up
            keyStates[0] = true;
            break;
        case SDLK_S:
            // Move player down
            keyStates[1] = true;
            break;
        case SDLK_A:
            // Move player left
            keyStates[2] = true;
            break;
        case SDLK_D:
            // Move player right
            keyStates[3] = true;
            break;
        default:
            break;
        }
    }

    // Process SDL events related to player control
    else if (sdlEvent.type == SDL_EVENT_KEY_UP)
    {
        // Handle key up events
        switch (sdlEvent.key.key)
        {
        case SDLK_W:
            // Move player up
            keyStates[0] = false;
            break;
        case SDLK_S:
            // Move player down
            keyStates[1] = false;
            break;
        case SDLK_A:
            // Move player left
            keyStates[2] = false;
            break;
        case SDLK_D:
            // Move player right
            keyStates[3] = false;
            break;
        default:
            break;
        }
    }

    else if (sdlEvent.type == SDL_EVENT_GAMEPAD_AXIS_MOTION)
    {
        // Handle gamepad button down events if needed
        if (sdlEvent.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX)
        {
            gamepadAxisValue.x = sdlEvent.gaxis.value;
        }
        else if (sdlEvent.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY)
        {
            gamepadAxisValue.y = sdlEvent.gaxis.value;
        }
    }
}

void PlayerControllerSystem::update(float deltaTime)
{

    // Update player position based on input
    glm::vec2 playerDirection = {0.0f, 0.0f};
    bool isMoving = false;
    if (gamepadConnected())
    {
        playerDirection.x = (std::abs(gamepadAxisValue.x) > gamepadAxisDeadZone) ? gamepadAxisValue.x / 32768.0f : 0.0f;
        playerDirection.y = (std::abs(gamepadAxisValue.y) > gamepadAxisDeadZone) ? gamepadAxisValue.y / 32768.0f : 0.0f;
        isMoving = (playerDirection.x != 0.0f || playerDirection.y != 0.0f);
    }
    else
    {
        if (keyStates[0]) // W
        {
            playerDirection.y -= 1.0f;
            isMoving = true;
        }
        if (keyStates[1]) // S
        {
            playerDirection.y += 1.0f;
            isMoving = true;
        }
        if (keyStates[2]) // A
        {
            playerDirection.x -= 1.0f;
            isMoving = true;
        }
        if (keyStates[3]) // D
        {
            playerDirection.x += 1.0f;
            isMoving = true;
        }
    }

    for (auto const &entity : getEntities())
    {
        try
        {
            auto &playerControl = world->getComponent<PlayerControlComponent>(entity);
            auto &velocity = world->getComponent<VelocityComponent>(entity);
            auto &animController = world->getComponent<AnimationControllerComponent>(entity);

            if (isMoving)
            {
                std::string currentState;
                if (playerDirection.y < 0)
                    currentState = "walk_up";
                else if (playerDirection.y > 0)
                    currentState = "walk_down";
                else if (playerDirection.x < 0)
                    currentState = "walk_left";
                else if (playerDirection.x > 0)
                    currentState = "walk_right";

                if (animController.currentState != currentState)
                {
                    world->publishEvent<EntityChangedStateEvent>(entity, currentState);
                }

                velocity.velocity = glm::normalize(playerDirection) * playerControl.speed;
            }
            else
            {
                velocity.velocity = {0.0f, 0.0f};
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "PlayerControllerSystem error accessing components: " << e.what() << std::endl;
        }
    }
}

void PlayerControllerSystem::init()
{
    SDL_Init(SDL_INIT_GAMEPAD);

    scanGamepad();
}

void PlayerControllerSystem::scanGamepad()
{
    int count = 0;
    SDL_JoystickID *ids = SDL_GetGamepads(&count);
    for (int i = 0; i < count; ++i)
    {
        if (connectGamePad(ids[i]))
            break;
    }
}

bool PlayerControllerSystem::gamepadConnected() const
{

    return gamepad != nullptr;
}

bool PlayerControllerSystem::connectGamePad(SDL_JoystickID id)
{
    SDL_Gamepad *gp = SDL_OpenGamepad(id);
    if (gp)
    {
        gamepad = gp;
        std::cout << "Gamepad connected: " << SDL_GetGamepadName(gp) << std::endl;
        return true;
    }
    return false;
}
