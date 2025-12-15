
add_executable(ecs main.cpp core/game_engine.cpp)
target_link_libraries(ecs SDL3::SDL3 glm::glm SDL3_image::SDL3_image) 


target_sources(ecs PRIVATE
    system/render_system.cpp
    system/player_controller_system.cpp
    core/world_config.cpp
    world_configs/default_world_config.cpp
)

