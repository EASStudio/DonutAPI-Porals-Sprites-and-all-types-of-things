#include "Player.h"
#include "Coordinator.h"

using namespace EntitySystems;

void Player::update(float dt)
{
    for (auto const& entity : entities)
    {
        auto& pos = coordinator->getComponent<Position>(entity);
    }
}