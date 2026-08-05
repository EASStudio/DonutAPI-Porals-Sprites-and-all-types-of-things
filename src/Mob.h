#pragma once

#include "System.h"
#include "Coordinator.h"
#include "Raycaster.h"  
#include "Textures.h"
#include "Music.h"

class Mob : public System
{
public:
    void update(float dt) override;
    void setPlayerEntity(Entity player);
    void draw(const Textures::TextureSet& texSet);
    int createMob(Coordinator& coord, float x, float y, int textureIndex);
    void setZBuffer(double* zb) { zBufferPtr = zb; }
    void setCoordinator(Coordinator* coord) { coordinator = coord; }

private:
    void updateAI(Entity entity, float dt);
    void pickWanderTarget(Entity entity, bool fleeFromPlayer);   
    bool moveTowards(Entity entity, float targetX, float targetY, float speed);
    bool moveAway(Entity entity, float targetX, float targetY, float speed);
    bool wonder(Entity entity, float targetX, float targetY, float speed);
    void drawSprite(Entity entity, const Textures::TextureSet& texSet);

    struct SpriteDrawInfo
    {
        Entity entity;
        float distance;
    };

private:
    Raycaster raycaster;
    double* zBufferPtr = nullptr;

};