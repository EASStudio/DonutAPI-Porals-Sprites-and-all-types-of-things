#include "Item.h"
#include "Coordinator.h"
#include "EntitySystems.h"

using namespace EntitySystems;

void Item::update(float dt)
{
    for (auto const& component : components)
    {
        if (coordinator->hasComponent<Weapon>(component))
        {
            auto& weapon = coordinator->getComponent<Weapon>(component);
            // TODO: Weapon logic
        }

        if (coordinator->hasComponent<Armor>(component))
        {
            auto& armor = coordinator->getComponent<Armor>(component);
            // TODO: Armor logic
        }

        if (coordinator->hasComponent<Potion>(component))
        {
            auto& armor = coordinator->getComponent<Armor>(component);
            // TODO: Potion logic
        }

        if (coordinator->hasComponent<Food>(component))
        {
            auto& armor = coordinator->getComponent<Armor>(component);
            // TODO: Food logic
        }
    }
}