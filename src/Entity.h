#pragma once

#include <array>
#include <bitset>
#include <queue>
#include <cstdint>

using Entity = std::uint32_t;
constexpr Entity MAX_ENTITIES = 10000; // 1,000,000 
constexpr Entity INVALID_ENTITY = 0xFFFFFFFF;

using ComponentType = std::uint8_t;
constexpr ComponentType MAX_COMPONENTS = 64;

using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager
{
public:
    EntityManager();

    Entity createEntity();
    void destroyEntity(Entity entity);

    void setSignature(Entity entity, Signature signature);
    Signature getSignature(Entity entity) const;

private:
    std::queue<Entity> availableEntities;
    std::array<Signature, MAX_ENTITIES> signatures;
    uint32_t livingEntityCount = 0;
};