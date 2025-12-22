#include "quadtree.h"
#include <queue>
#include <iostream>

Quadtree::Quadtree(float x, float y, float width, float height, int maxCollidersPerNode): 
    root(new QuadtreeNode(x, y, width, height)), maxCollidersPerNode(maxCollidersPerNode)
{
}
Quadtree::~Quadtree()
{
    // Properly delete all nodes to avoid memory leaks
    // (Implementation of node deletion is omitted for brevity)
    deleteNode(root);
}

void Quadtree::insertRectangle(const Entity &entity, float x, float y, float width, float height)
{
    QuadtreeEntity collider{entity, QuadtreeCollider( x, y, width, height)};
    insert(collider);
}

void Quadtree::insertPoint(const Entity &entity, float x, float y)
{
    QuadtreeEntity collider{entity, QuadtreeCollider( x, y)};
    insert(collider);
}

void Quadtree::insertCircle(const Entity &entity, float centerX, float centerY, float radius)
{
    QuadtreeEntity collider{entity, QuadtreeCollider( centerX, centerY, radius)};
    insert(collider);
}

void Quadtree::remove(const Entity &entity)
{
    auto it = entityNodeMap.find(entity);
    if(it != entityNodeMap.end())
    {
        for(auto node : it->second)
        {
            auto &colliders = node->colliders;
            for(auto colliderIt = colliders.begin(); colliderIt != colliders.end(); )
            {
                if(colliderIt->entity == entity)
                {
                    colliderIt = colliders.erase(colliderIt);
                }
                else
                {
                    ++colliderIt;
                }
            }
        }
        entityNodeMap.erase(it);
    }
}

void Quadtree::clear()
{
    clearNode(root);
    root->isLeaf = true;
}

void Quadtree::queryRectangle(float x, float y, float width, float height,std::set<Entity> &result)
{
    QuadtreeCollider area( x, y, width, height);


    query(area, result);
   

}

void Quadtree::queryPoint(float x, float y,std::set<Entity> &result)
{
    QuadtreeCollider area( x, y);
    query(area, result);
}

void Quadtree::queryCircle(float centerX, float centerY, float radius,std::set<Entity> &result)
{
    QuadtreeCollider area(centerX, centerY, radius);
    query(area, result);

}

void Quadtree::debugPrint()
{
    debugPrint(root, 0);
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

bool Quadtree::isContained(const QuadtreeNode *node, const QuadtreeCollider &a) const
{
    switch(a.type)
    {
        case QuadtreeCollider::Type::Rectangle:
            return (a.x >= node->x && a.x + a.width <= node->x + node->width &&
                    a.y >= node->y && a.y + a.height <= node->y + node->height);
        case QuadtreeCollider::Type::Circle:
            return (a.x - a.radius >= node->x && a.x + a.radius <= node->x + node->width &&
                    a.y - a.radius >= node->y && a.y + a.radius <= node->y + node->height);
        case QuadtreeCollider::Type::Point:
            return (a.x >= node->x && a.x <= node->x + node->width &&
                    a.y >= node->y && a.y <= node->y + node->height);
        default:
            assert(false && "Unknown QuadtreeCollider type");
            return false;
    }
    return false;
}

bool Quadtree::isColling(const QuadtreeCollider &a, const QuadtreeCollider &b) const
{
    return false;
}

void Quadtree::insert(QuadtreeEntity &collider)
{   
    while(!isContained(root, collider.collider))
    {
        std::cout << "Collider out of bounds, expanding quadtree." << std::endl;
        expand(collider.collider.x, collider.collider.y);
    }
    checkNode(root, collider);
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

    //Remove all quadtreecolliders from node, and entitymapping and replace those colliders in children and remap entity

    for(auto collider : node->colliders){
        entityNodeMap[collider.entity].clear();
        for(int i = 0; i < 4; ++i){
           checkNode(node->children[i], collider);
        } 
    }
    node->colliders.clear();
    

}

void Quadtree::expand(float x, float y)
{
    float midX = x + root->width / 2.0f;
    float midY = y + root->height / 2.0f;

    if ( x < midX && y < midY) {
        // Expand to top-left
        std::cout << "Expanding Quadtree to top-left." << std::endl;
        QuadtreeNode* newRoot = new QuadtreeNode(root->x - root->width, root->y - root->height, root->width * 2, root->height * 2);
        newRoot->children[0] = new QuadtreeNode(newRoot->x, newRoot->y, root->width, root->height);
        newRoot->children[1] = new QuadtreeNode(newRoot->x + root->width, newRoot->y, root->width, root->height);
        newRoot->children[2] = new QuadtreeNode(newRoot->x, newRoot->y + root->height, root->width, root->height);
        newRoot->children[3] = root;
        newRoot->isLeaf = false;
        root = newRoot;
    }else if (x >= midX && y < midY) {
        // Expand to top-right
        std::cout << "Expanding Quadtree to top-right." << std::endl;
        QuadtreeNode* newRoot = new QuadtreeNode(root->x, root->y - root->height, root->width * 2, root->height * 2);
        newRoot->children[0] = new QuadtreeNode(newRoot->x, newRoot->y, root->width, root->height);
        newRoot->children[1] = new QuadtreeNode(newRoot->x + root->width, newRoot->y, root->width, root->height);
        newRoot->children[2] = root;
        newRoot->children[3] = new QuadtreeNode(newRoot->x + root->width, newRoot->y + root->height, root->width, root->height);
        newRoot->isLeaf = false;
        root = newRoot;
    }else if (x < midX && y >= midY) {
        // Expand to bottom-left
        std::cout << "Expanding Quadtree to bottom-left." << std::endl;
        QuadtreeNode* newRoot = new QuadtreeNode(root->x - root->width, root->y, root->width * 2, root->height * 2);
        newRoot->children[0] = new QuadtreeNode(newRoot->x, newRoot->y, root->width, root->height);
        newRoot->children[1] = root;
        newRoot->children[2] = new QuadtreeNode(newRoot->x, newRoot->y + root->height, root->width, root->height);
        newRoot->children[3] = new QuadtreeNode(newRoot->x + root->width, newRoot->y + root->height, root->width, root->height);
        newRoot->isLeaf = false;
        root = newRoot;
    } else {
        // Expand to bottom-right
        std::cout << "Expanding Quadtree to bottom-right." << std::endl;
        QuadtreeNode* newRoot = new QuadtreeNode(root->x, root->y, root->width * 2, root->height * 2);
        newRoot->children[0] = root;
        newRoot->children[1] = new QuadtreeNode(newRoot->x + root->width, newRoot->y, root->width, root->height);
        newRoot->children[2] = new QuadtreeNode(newRoot->x, newRoot->y + root->height, root->width, root->height);
        newRoot->children[3] = new QuadtreeNode(newRoot->x + root->width, newRoot->y + root->height, root->width, root->height);
        newRoot->isLeaf = false;
        root = newRoot;
    }
}

void Quadtree::insertIntoNode(QuadtreeNode *node, QuadtreeEntity &collider)
{
    node->colliders.insert(std::move(collider));
    entityNodeMap[collider.entity].push_back(node);

    if(node->colliders.size() > maxCollidersPerNode)
    {
        std::cout << "Splitting node at (" << node->x << ", " << node->y << ") with " << node->colliders.size() << " colliders." << std::endl;  
        splitNode(node);
    }
}

void Quadtree::checkNode(QuadtreeNode *node, QuadtreeEntity &area)
{
    if(isColliding(node, area.collider))
    {
        if(node->isLeaf)
        {
            insertIntoNode(node, area);
        }
        else
        {
            for(int i = 0; i < 4; ++i)
            {
                checkNode(node->children[i], area);
            }
        }
    }
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

void Quadtree::query(QuadtreeCollider &area, std::set<Entity> &result)
{
     std::queue<QuadtreeNode*> nodesToCheck;
    nodesToCheck.push(root);

    while(!nodesToCheck.empty())
    {
        QuadtreeNode *currentNode = nodesToCheck.front();
        nodesToCheck.pop();

        if(isColliding(currentNode, area))
        {

            if(currentNode->isLeaf)
            {
                for(const auto &collider : currentNode->colliders)
                {
                    
                    result.insert(collider.entity);
                }
            }
            else
            {
                for(int i = 0; i < 4; ++i)
                {
                    nodesToCheck.push(currentNode->children[i]);
                }
            }
        }
    }
          
}

void Quadtree::debugPrint(QuadtreeNode *node, int depth)
{
    for(int i = 0; i < depth; ++i)
        std::cout << "  ";
    std::cout << "Node " << (node->isLeaf ? "(Leaf) ":"")  <<"at (" << node->x << ", " << node->y << "), size (" << node->width << "x" << node->height << "), colliders: " << node->colliders.size() << std::endl;

    if(!node->isLeaf)
    {
        for(int i = 0; i < 4; ++i)
        {
            debugPrint(node->children[i], depth + 1);
        }
    }
}
