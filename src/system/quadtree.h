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

    QuadtreeCollider( float x, float y, float width, float height) : type(Type::Rectangle), x(x), y(y), width(width), height(height), radius(0.0f) {}
    QuadtreeCollider( float centerX, float centerY, float radius) : type(Type::Circle), x(centerX), y(centerY), width(0.0f), height(0.0f), radius(radius) {}
    QuadtreeCollider( float x, float y) : type(Type::Point), x(x), y(y), width(0.0f), height(0.0f), radius(0.0f) {}


};

struct QuadtreeEntity{
    Entity entity;
    QuadtreeCollider collider;

    bool operator<(const QuadtreeEntity &other) const
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

    std::set<QuadtreeEntity> colliders;
};

class Quadtree
{

public:
    Quadtree(float x, float y, float width, float height, int maxCollidersPerNode = 4);
    ~Quadtree();

    void insertRectangle(const Entity &entity, float x, float y, float width, float height);
    void insertPoint(const Entity &entity, float x, float y);
    void insertCircle(const Entity &entity, float centerX, float centerY, float radius);

    void remove(const Entity &entity);

    void clear();

    void queryRectangle(float x, float y, float width, float height,std::set<Entity> &result);
    void queryPoint(float x, float y,std::set<Entity> &result);
    void queryCircle(float centerX, float centerY, float radius,std::set<Entity> &result);

    void debugPrint();

private:
    QuadtreeNode *root;

    int maxCollidersPerNode =4;

    std::unordered_map<Entity, std::vector<QuadtreeNode *>> entityNodeMap;

    void insert(QuadtreeEntity &collider);

    void splitNode(QuadtreeNode *node);
    void expand(float x, float y);
    void insertIntoNode(QuadtreeNode *node, QuadtreeEntity& collider);

    void checkNode(QuadtreeNode * node, QuadtreeEntity & area);

    void deleteNode(QuadtreeNode *node);

    bool inNode(const QuadtreeNode *node, const QuadtreeCollider &a) const;
    bool isContained(const QuadtreeNode *node, const QuadtreeCollider &a) const;
    bool isColliding(const QuadtreeCollider &a, const QuadtreeCollider &b) const;
    bool inNodeRect(const QuadtreeNode *node, float x, float y, float w, float h) const;
    bool inNodeCircle(const QuadtreeNode *node, float x, float y, float radius) const;
    bool inNodePoint(const QuadtreeNode *node, float x, float y) const;

    void query(QuadtreeCollider &area, std::set<Entity> &result);

    void debugPrint(QuadtreeNode *node, int depth = 0);


    void clearNode(QuadtreeNode *node)
    {
        node->colliders.clear();
        if(!node->isLeaf)
        {
            for(int i = 0; i < 4; ++i)
            {
                clearNode(node->children[i]);
                delete node->children[i];
                node->children[i] = nullptr;
            }
        }
    }

    bool isCollidingRectRect(const QuadtreeCollider &a, const QuadtreeCollider &b) const
    {
        return !(a.x + a.width < b.x || b.x + b.width < a.x ||
                 a.y + a.height < b.y || b.y + b.height < a.y);
    }

    bool isCollidingCircleCircle(const QuadtreeCollider &a, const QuadtreeCollider &b) const
    {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        float radiiSum = a.radius + b.radius;
        return (dx * dx + dy * dy) < (radiiSum * radiiSum);
    }

    bool isCollidingRectCircle(const QuadtreeCollider &a, const QuadtreeCollider &b) const
    {
        float closestX = std::max(a.x, std::min(b.x, a.x + a.width));
        float closestY = std::max(a.y, std::min(b.y, a.y + a.height));

        float dx = b.x - closestX;
        float dy = b.y - closestY;

        return (dx * dx + dy * dy) < (b.radius * b.radius);
    }

    bool isCollidingRectPoint(const QuadtreeCollider &a, const QuadtreeCollider &b) const
    {
        return (b.x >= a.x && b.x <= a.x + a.width &&
                b.y >= a.y && b.y <= a.y + a.height);
    }

    bool isCollidingCirclePoint(const QuadtreeCollider &a, const QuadtreeCollider &b) const
    {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return (dx * dx + dy * dy) < (a.radius * a.radius);
    }

    bool isCollidingPointPoint(const QuadtreeCollider &a, const QuadtreeCollider &b) const
    {
        return a.x == b.x && a.y == b.y;
    }

};