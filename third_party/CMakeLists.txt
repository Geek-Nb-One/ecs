add_subdirectory(SDL)
add_subdirectory(glm)
add_subdirectory(SDL_image)


add_library(imgui STATIC)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

target_sources(imgui PRIVATE 
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_impl_sdl3.cpp
    imgui/imgui_impl_sdlrenderer3.cpp
    imgui/imgui_tables.cpp
)

target_link_libraries(imgui PUBLIC SDL3::SDL3)


