# Working with Optional Components in ECS

## Problem
Some entities in your system have certain components (like `VelocityComponent`) while others don't. If you try to access a component that doesn't exist, your program will crash.

## Solution
Use `world->hasComponent<ComponentType>(entity)` to check if an entity has a component before accessing it.

## Example Usage

### In ColliderSystem
The ColliderSystem requires `ColliderComponent` and `Transform` (set via signature), but `VelocityComponent` is optional:

```cpp
void ColliderSystem::resolveCollisions(std::set<std::pair<Entity, Entity>> &handledPairs)
{
    for (const auto &pair : handledPairs)
    {
        Entity a = pair.first;
        Entity b = pair.second;

        // These components are guaranteed to exist (part of signature)
        auto &colliderA = world->getComponent<ColliderComponent>(a);
        auto &transformA = world->getComponent<Transform>(a);

        // Check if optional component exists before accessing
        bool aHasVelocity = world->hasComponent<VelocityComponent>(a);
        
        if (aHasVelocity)
        {
            // Safe to access now
            auto &velocityA = world->getComponent<VelocityComponent>(a);
            velocityA.velocity.x = 0.0f;
        }
    }
}
```

### General Pattern

```cpp
// Check if entity has the component
if (world->hasComponent<ComponentType>(entity))
{
    // Safe to get the component
    auto &component = world->getComponent<ComponentType>(entity);
    
    // Use the component
    component.someValue = 100.0f;
}
```

### When to Use

1. **System Signature**: Define required components that ALL entities in the system must have
   ```cpp
   Signature signature;
   signature.set(world->getComponentType<Transform>().id);
   signature.set(world->getComponentType<ColliderComponent>().id);
   world->setSystemSignature<ColliderSystem>(signature);
   ```

2. **Optional Components**: Check with `hasComponent()` for components that only SOME entities have
   - Movement/Velocity (some objects are static)
   - Animation (not all sprites are animated)
   - AI/Player control (some entities are passive)
   - Special effects or abilities

### Benefits

- **No crashes**: Safe component access
- **Flexible systems**: Systems can handle entities with different component combinations
- **Better performance**: Only process relevant components when they exist
- **Cleaner code**: Explicit about what's required vs optional

### Performance Note

The `hasComponent()` check is very fast (O(1) hash map lookup), so don't worry about calling it frequently.
