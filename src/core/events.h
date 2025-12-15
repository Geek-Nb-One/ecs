#pragma once

#include <string>

struct EntityChangedStateEvent{
    Entity entity;
    std::string stateName;
    EntityChangedStateEvent(Entity e, const std::string& state): entity(e), stateName(state) {}
};

struct QuitEvent{};

struct WindowResizeRequestedEvent{
    int width;
    int height;

    WindowResizeRequestedEvent(int w, int h): width(w), height(h) {}
};

struct WindowFullScreenRequestedEvent{
    bool isFullScreen;

    WindowFullScreenRequestedEvent(bool fullScreen): isFullScreen(fullScreen) {}
};

struct LogicalSizeChangedEvent{
    int width;
    int height;

    LogicalSizeChangedEvent(int w, int h): width(w), height(h) {}
};

struct AxisMovedEvent{
    float xAxis;
    float yAxis;

    AxisMovedEvent(float x, float y): xAxis(x), yAxis(y) {}
};

struct ButtonPressedEvent{
    int buttonId;

    ButtonPressedEvent(int id): buttonId(id) {}
};