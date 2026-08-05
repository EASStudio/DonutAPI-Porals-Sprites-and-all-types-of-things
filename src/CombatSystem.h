#pragma once

#include "System.h"
#include "Coordinator.h"

class CombatSystem : public System
{
public:
    void update(float dt) override;
    void attack(Entity attacker, Entity target);
};