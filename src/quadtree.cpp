#include "quadtree.h"

Quadtree::Quadtree(float x, float y, float width, float height): 
    root(new QuadtreeNode(x, y, width, height))
{
}
Quadtree::~Quadtree()
{
    // Properly delete all nodes to avoid memory leaks
    // (Implementation of node deletion is omitted for brevity)
    deleteNode(root);
}

bool Quadtree::isColliding(const QuadtreeNode *node, const QuadtreeCollider &a) const
{
    switch(a.type)
    {
        case QuadtreeCollider::Type::Rectangle:
            return isCollindingRect(node, a.x, a.y, a.width, a.height);
        case QuadtreeCollider::Type::Circle:
            return isCollindingCircle(node, a.x, a.y, a.radius);
        case QuadtreeCollider::Type::Point:
            return isCollindingPoint(node, a.x, a.y);
        default:
            assert(false && "Unknown QuadtreeCollider type");
            return false;
    }

}

void Quadtree::splitNode(QuadtreeNode *node)
{
    float hx = node->x + node->width / 2.0f;
    float hy = node->y + node->height / 2.0f;
    float hw = node->width / 2.0f;
    float hh = node->height / 2.0f;

    node->children[0] = new QuadtreeNode(node->x, node->y, hw, hh);         // Top-left
    node->children[1] = new QuadtreeNode(hx, node->y, hw, hh);              // Top-right
    node->children[2] = new QuadtreeNode(node->x, hy, hw, hh);              // Bottom-left
    node->children[3] = new QuadtreeNode(hx, hy, hw, hh);                   // Bottom-right

    node->isLeaf = false;
    //TODO
    //Remove all quadtreecolliders from node, and entitymapping and replace those colliders in children and remap entity

}

void Quadtree::insertIntoNode(QuadtreeNode *node, QuadtreeCollider &collider)
{
    node->entities.insert(std::move(collider));
    entityNodeMap[collider.entity].push_back(node);
}



void Quadtree::deleteNode(QuadtreeNode *node)
{
    for(auto child : node->children)
    {
        if(child)
        {
            deleteNode(child);
        }
    }
    delete node;
}

bool Quadtree::isCollindingRect(const QuadtreeNode *node, float x, float y, float w, float h) const
{
    return !(node->x + node->width < x || x + w < node->x ||
             node->y + node->height < y || y + h < node->y);

}

bool Quadtree::isCollindingCircle(const QuadtreeNode *node, float x, float y, float radius) const
{
    float closestX = std::max(node->x, std::min(x, node->x + node->width));
    float closestY = std::max(node->y, std::min(y, node->y + node->height));

    float distanceX = x - closestX;
    float distanceY = y - closestY;

    return (distanceX * distanceX + distanceY * distanceY) < (radius * radius);

}

bool Quadtree::isCollindingPoint(const QuadtreeNode *node, float x, float y) const
{
    return (x >= node->x && x <= node->x + node->width &&
        y >= node->y && y <= node->y + node->height);
}
