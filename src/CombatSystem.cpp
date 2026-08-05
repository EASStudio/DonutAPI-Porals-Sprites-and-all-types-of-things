#include "CombatSystem.h"
#include "Coordinator.h"
#include "EntitySystems.h"

void CombatSystem::update(float dt)
{
    // Could process auto-attacks, cooldowns
}

void CombatSystem::attack(Entity attacker, Entity target)
{
    if (!coordinator->hasComponent<EntitySystems::Attack>(attacker) || !coordinator->hasComponent<EntitySystems::Health>(target))
        return;

    auto& atk = coordinator->getComponent<EntitySystems::Attack>(attacker);
    auto& def = coordinator->getComponent<EntitySystems::Defense>(target); 
    auto& hp = coordinator->getComponent<EntitySystems::Health>(target);

    int damage = std::max(1, atk.value - (def.value / 2));

    hp.currentHp -= damage;

    if (hp.currentHp <= 0)
        coordinator->destroyEntity(target);
}