#pragma once



struct QuitEvent{};

struct WindowResizeRequestedEvent{
    int width;
    int height;

    WindowResizeRequestedEvent(int w, int h): width(w), height(h) {}
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