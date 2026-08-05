#pragma once

#include "Entity.h"

#include <vector>

class Coordinator;  

class System
{
public:
    virtual ~System() = default;
    virtual void update(float dt) {}

    std::vector<Entity> entities;
    std::vector<ComponentType> components;
    Coordinator* coordinator = nullptr;
};