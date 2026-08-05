#include "Entity.h"

#include <iostream>
#include <cassert>

EntityManager::EntityManager()
{
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
        availableEntities.push(entity);
}

Entity EntityManager::createEntity()
{
    assert(livingEntityCount < MAX_ENTITIES && "Too many entities!");

    Entity id = availableEntities.front();
    availableEntities.pop();
    ++livingEntityCount;

    signatures[id].reset();        
    return id;
}

void EntityManager::destroyEntity(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");

    signatures[entity].reset();
    availableEntities.push(entity);
    --livingEntityCount;
}

void EntityManager::setSignature(Entity entity, Signature signature)
{
    assert(entity < MAX_ENTITIES);

    signatures[entity] = signature;
}

Signature EntityManager::getSignature(Entity entity) const
{
    assert(entity < MAX_ENTITIES);

    return signatures[entity];
}