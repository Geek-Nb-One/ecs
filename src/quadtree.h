#pragma once

#include "core/entity_manager.h"
#include <unordered_map>

struct QuadtreeCollider
{
    enum class Type
    {
        Rectangle,
        Circle,
        Point
    } type;

    float x, y, width, height, radius;
    Entity entity;

    QuadtreeCollider(Entity entity, float x, float y, float width, float height) : entity(entity), type(Type::Rectangle), x(x), y(y), width(width), height(height), radius(0.0f) {}
    QuadtreeCollider(Entity entity, float centerX, float centerY, float radius) : entity(entity), type(Type::Circle), x(centerX), y(centerY), width(0.0f), height(0.0f), radius(radius) {}
    QuadtreeCollider(Entity entity, float x, float y) : entity(entity), type(Type::Point), x(x), y(y), width(0.0f), height(0.0f), radius(0.0f) {}

    bool operator<(const QuadtreeCollider &other) const
    {
        return entity < other.entity;
    }
};

struct QuadtreeNode
{
    float x, y, width, height;
    QuadtreeNode *children[4];
    bool isLeaf;
    // Additional data can be added here

    QuadtreeNode(float x, float y, float width, float height) : x(x), y(y), width(width), height(height), isLeaf(true)
    {
        children[0] = children[1] = children[2] = children[3] = nullptr;
    }

    std::set<QuadtreeCollider> entities;
};

class Quadtree
{

public:
    Quadtree(float x, float y, float width, float height);
    ~Quadtree();

    void insertRectangle(const Entity &entity, float x, float y, float width, float height);
    void insertPoint(const Entity &entity, float x, float y);
    void insertCircle(const Entity &entity, float centerX, float centerY, float radius);

    void remove(const Entity &entity);

    std::set<Entity> queryRectangle(float x, float y, float width, float height);
    std::set<Entity> queryPoint(float x, float y);
    std::set<Entity> queryCircle(float centerX, float centerY, float radius);

private:
    QuadtreeNode *root;

    std::unordered_map<Entity, std::vector<QuadtreeNode *>> entityNodeMap;

    void splitNode(QuadtreeNode *node);
    void insertIntoNode(QuadtreeNode *node, QuadtreeCollider& collider);

    void deleteNode(QuadtreeNode *node);

    bool isColliding(const QuadtreeNode *node, const QuadtreeCollider &a) const;
    bool isCollindingRect(const QuadtreeNode *node, float x, float y, float w, float h) const;
    bool isCollindingCircle(const QuadtreeNode *node, float x, float y, float radius) const;
    bool isCollindingPoint(const QuadtreeNode *node, float x, float y) const;
};